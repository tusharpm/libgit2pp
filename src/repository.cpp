/******************************************************************************
 * This file is part of the libgit2pp library
 * Copyright (c) 2011 Laszlo Papp <djszapi@archlinux.us>
 * Copyright (C) 2013 Leonardo Giordani
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include "git2pp/repository.h"
#include "git2pp/config.h"
#include "git2pp/tag.h"
#include "git2pp/blob.h"
#include "git2pp/signature.h"
#include "git2pp/exception.h"
#include "git2pp/status.h"
#include "git2pp/remote.h"
#include "git2pp/credentials.h"
#include "git2pp/diff.h"
#include "private/annotatedcommit.h"
#include "private/buffer.h"
#include "private/helpers.h"
#include "private/pathcodec.h"
#include "private/remotecallbacks.h"
#include "private/strarray.h"
#include <filesystem>
#include <map>

namespace {
    void do_not_free(git_repository*) {}
}

namespace LibGit2pp
{


class Repository::Private : public internal::RemoteListener
{
public:
    typedef std::shared_ptr<git_repository> ptr_type;
    ptr_type d;
    std::map<std::string, Credentials> m_remote_credentials;
    Repository &m_owner;

    Private(git_repository *repository, bool own, Repository &owner) :
        d(repository, own ? git_repository_free : do_not_free),
        m_owner(owner)
    {
    }

    Private(const Private &other, Repository &owner) :
        d(other.d),
        m_remote_credentials(other.m_remote_credentials),
        m_owner(owner)
    {
    }

    void init(const std::string& path, bool isBare)
    {
        d.reset();
        git_repository *repo = 0;
        qGitThrow(git_repository_init(&repo, path.c_str(), isBare));
        setData(repo);
    }

    void open(const std::string& path)
    {
        d.reset();
        git_repository *repo = 0;
        qGitThrow(git_repository_open(&repo, path.c_str()));
        setData(repo);
    }

    void setData(git_repository *repo)
    {
        d = ptr_type(repo, git_repository_free);
    }

    git_repository* safeData(const char *funcName) const {
        if (!d){
            throw Exception("Repository::" + std::string(funcName) + "(): no repository available");
        }
        return d.get();
    }

    int progress(int percent)
    {
        m_owner.cloneProgress(percent);
        return 0;
    }
};


#define SAFE_DATA d_ptr->safeData(LIBGIT2PP_FUNC_NAME)

#define THROW(msg) throw Exception(std::string("Repository::") + LIBGIT2PP_FUNC_NAME + "(): " + msg)

#define AVOID(statement, msg) if (statement) {\
    THROW(msg);\
}


Repository::Repository(git_repository *repository, bool own)
    : d_ptr(new Private(repository, own, *this))
{
}

Repository::Repository(const Repository& other)
    : d_ptr(new Private(*other.d_ptr, *this))
{
}

Repository::~Repository()
{
}

std::string Repository::discover(const std::string& startPath, bool acrossFs, const std::vector<std::string>& ceilingDirs)
{
    internal::Buffer repoPath;
    std::string delimiter{1, GIT_PATH_LIST_SEPARATOR};
    auto joinedCeilingDirs = join(ceilingDirs, delimiter);
    qGitThrow(git_repository_discover(repoPath.data(), startPath.c_str(), acrossFs, joinedCeilingDirs.c_str()));

    return repoPath.asPath();
}

void Repository::init(const std::string& path, bool isBare)
{
    d_ptr->init(path, isBare);
}

void Repository::open(const std::string& path)
{
    d_ptr->open(path);
}

void Repository::discoverAndOpen(const std::string &startPath, bool acrossFs,
                                 const std::vector<std::string> &ceilingDirs)
{
    open(discover(startPath, acrossFs, ceilingDirs));
}

Reference Repository::head() const
{
    git_reference *ref = 0;
    qGitThrow(git_repository_head(&ref, SAFE_DATA));
    return Reference(ref);
}

bool Repository::isHeadDetached() const
{
    return qGitThrow(git_repository_head_detached(SAFE_DATA)) == 1;
}

bool Repository::isHeadUnborn() const
{
    return qGitThrow(git_repository_head_unborn(SAFE_DATA)) == 1;
}

bool Repository::isEmpty() const
{
    return qGitThrow(git_repository_is_empty(SAFE_DATA)) == 1;
}

bool Repository::isBare() const
{
    return qGitThrow(git_repository_is_bare(SAFE_DATA)) == 1;
}

std::string Repository::name() const
{
    auto repoPath = std::filesystem::path{workDirPath()}.lexically_normal();
    if (isBare())
        repoPath = std::filesystem::path{path()}.lexically_normal();

    return repoPath.filename();
}

std::string Repository::path() const
{
    return git_repository_path(SAFE_DATA);
}

std::string Repository::workDirPath() const
{
    return git_repository_workdir(SAFE_DATA);
}

Config Repository::configuration() const
{
    git_config *cfg;
    qGitThrow(git_repository_config(&cfg, SAFE_DATA));
    return Config(cfg);
}

Reference Repository::lookupRef(const std::string& name) const
{
    git_reference *ref = 0;
    qGitThrow(git_reference_lookup(&ref, SAFE_DATA, name.c_str()));
    return Reference(ref);
}

OId Repository::lookupRefOId(const std::string& name) const
{
    git_oid oid;
    qGitThrow(git_reference_name_to_id(&oid, SAFE_DATA, name.c_str()));
    return OId(&oid);
}

Reference Repository::lookupShorthandRef(const std::string& shorthand) const
{
    git_reference *ref = 0;
    qGitThrow(git_reference_dwim(&ref, SAFE_DATA, shorthand.c_str()));
    return Reference(ref);
}

Commit Repository::lookupCommit(const OId& oid) const
{
    git_commit *commit = 0;
    qGitThrow(git_commit_lookup_prefix(&commit, SAFE_DATA, oid.constData(), oid.length()));
    return Commit(commit);
}

Tag Repository::lookupTag(const OId& oid) const
{
    git_tag *tag = 0;
    qGitThrow(git_tag_lookup_prefix(&tag, SAFE_DATA, oid.constData(), oid.length()));
    return Tag(tag);
}

Tree Repository::lookupTree(const OId& oid) const
{
    git_tree *tree = 0;
    qGitThrow(git_tree_lookup_prefix(&tree, SAFE_DATA, oid.constData(), oid.length()));
    return Tree(tree);
}

Blob Repository::lookupBlob(const OId& oid) const
{
    git_blob *blob = 0;
    qGitThrow(git_blob_lookup_prefix(&blob, SAFE_DATA, oid.constData(), oid.length()));
    return Blob(blob);
}

Object Repository::lookupAny(const OId &oid) const
{
    git_object *object = 0;
    qGitThrow(git_object_lookup_prefix(&object, SAFE_DATA, oid.constData(), oid.length(), GIT_OBJ_ANY));
    return Object(object);
}

Object Repository::lookupRevision(const std::string &revspec) const
{
    git_object *object = 0;
    qGitThrow(git_revparse_single(&object, SAFE_DATA, revspec.c_str()));
    return Object(object);
}

Reference Repository::createRef(const std::string& name, const LibGit2pp::OId& oid, bool overwrite, const std::string &message)
{
    git_reference *ref = 0;
    qGitThrow(git_reference_create(&ref, SAFE_DATA, name.c_str(), oid.constData(), overwrite, message.c_str()));
    return Reference(ref);
}

Reference Repository::createSymbolicRef(const std::string& name, const std::string& target, bool overwrite, const std::string &message)
{
    git_reference *ref = 0;
    qGitThrow(git_reference_symbolic_create(&ref, SAFE_DATA, name.c_str(), target.c_str(), overwrite, message.c_str()));
    return Reference(ref);
}

OId Repository::createCommit(const Tree& tree, const std::list<Commit>& parents, const Signature& author, const Signature& committer, const std::string& message, const std::string &ref)
{
    std::vector<const git_commit*> p;
    for (const Commit& parent : parents) {
        p.push_back(parent.data());
    }

    OId oid;
    qGitThrow(git_commit_create(oid.data(), SAFE_DATA, ref.empty() ? NULL : ref.c_str(), author.data(), committer.data(),
                                NULL, message.c_str(), tree.data(), p.size(), p.data()));
    return oid;
}

OId Repository::createTag(const std::string& name,
                                  const Object& target,
                                  bool overwrite)
{
    OId oid;
    qGitThrow(git_tag_create_lightweight(oid.data(), SAFE_DATA, name.c_str(),
                                         target.data(), overwrite));
    return oid;
}

OId Repository::createTag(const std::string& name,
                                  const Object& target,
                                  const Signature& tagger,
                                  const std::string& message,
                                  bool overwrite)
{
    OId oid;
    qGitThrow(git_tag_create(oid.data(), SAFE_DATA, name.c_str(), target.data(),
                             tagger.data(), message.c_str(), overwrite));
    return oid;
}

void Repository::deleteTag(const std::string& name)
{
    qGitThrow(git_tag_delete(SAFE_DATA, name.c_str()));
}

OId Repository::createBlobFromFile(const std::string& path)
{
    OId oid;
    qGitThrow(git_blob_create_fromdisk(oid.data(), SAFE_DATA, path.c_str()));
    return oid;
}

OId Repository::createBlobFromBuffer(const QByteArray& buffer)
{
    OId oid;
    qGitThrow(git_blob_create_frombuffer(oid.data(), SAFE_DATA, buffer.data(), buffer.size()));
    return oid;
}

Reference Repository::createBranch(const std::string &branchName, const Commit &target, bool force)
{
    Commit usedTarget(target);
    if (target.isNull()) {
        usedTarget = lookupCommit(head().target());
    }

    git_reference *ref = NULL;
    qGitThrow(git_branch_create(&ref, SAFE_DATA, branchName.c_str(), usedTarget.data(), force));
    return Reference(ref);
}

void Repository::deleteBranch(const std::string &branchName)
{
    Reference branch = lookupShorthandRef(branchName);
    qGitThrow(git_branch_delete(branch.data()));
}

void Repository::cherryPick(const Commit &commit, const CherryPickOptions &opts)
{
    AVOID(commit.isNull(), "can not cherry-pick a null commit.")

    qGitThrow(git_cherrypick(SAFE_DATA, commit.data(), opts.data()));
}

std::vector<std::string> Repository::listTags(const std::string& pattern) const
{
    git_strarray tags;
    qGitThrow(git_tag_list_match(&tags, pattern.c_str(), SAFE_DATA));
    std::vector<std::string> list;
    for (size_t i = 0; i < tags.count; ++i)
    {
        list.push_back(tags.strings[i]);
    }
    git_strarray_free(&tags);
    return list;
}

std::vector<std::string> Repository::listReferences() const
{
    git_strarray refs;
    qGitThrow(git_reference_list(&refs, SAFE_DATA));
    std::vector<std::string> list;
    for (size_t i = 0; i < refs.count; ++i)
    {
        list.push_back(refs.strings[i]);
    }
    git_strarray_free(&refs);
    return list;
}

Database Repository::database() const
{
    git_odb *odb;
    qGitThrow( git_repository_odb(&odb, SAFE_DATA) );
    return Database(odb);
}

Index Repository::index() const
{
    git_index *idx;
    qGitThrow(git_repository_index(&idx, SAFE_DATA));
    return Index(idx);
}

StatusList Repository::status(const StatusOptions &options) const
{
    const git_status_options opt = options.constData();
    git_status_list *status_list;
    qGitThrow(git_status_list_new(&status_list, SAFE_DATA, &opt));
    return StatusList(status_list);
}

Repository::GraphRelationship Repository::commitRelationship(const Commit &local, const Commit &upstream) const
{
    GraphRelationship result;
    qGitThrow(git_graph_ahead_behind(&result.ahead, &result.behind, SAFE_DATA, local.oid().constData(), upstream.oid().constData()));
    return result;
}

Diff Repository::diffTrees(const Tree &oldTree, const Tree &newTree) const
{
    git_diff *diff = NULL;
    qGitThrow(git_diff_tree_to_tree(&diff, SAFE_DATA, oldTree.data(), newTree.data(), NULL));
    return Diff(diff);
}

Commit Repository::mergeBase(const Commit &one, const Commit &two) const
{
    OId out;
    qGitThrow(git_merge_base(out.data(), SAFE_DATA, one.oid().constData(), two.oid().constData()));
    return lookupCommit(out);
}

Index Repository::mergeTrees(const Tree &our, const Tree &their, const Tree &ancestor, const MergeOptions &opts)
{
    AVOID(our.isNull() && their.isNull(), "needed at least either 'our' or 'their' tree to merge.")

    git_index *index = NULL;
    qGitThrow(git_merge_trees(&index, SAFE_DATA, ancestor.data(), our.data(), their.data(), opts.data()));
    return Index(index);
}

git_repository* Repository::data() const
{
    return d_ptr->d.get();
}

const git_repository* Repository::constData() const
{
    return d_ptr->d.get();
}


void Repository::setRemoteCredentials(const std::string& remoteName, Credentials credentials)
{
    d_ptr->m_remote_credentials[remoteName] = credentials;
}


void Repository::clone(const std::string& url, const std::string& path)
{
    const std::string remoteName("origin");
    internal::RemoteCallbacks remoteCallbacks(d_ptr.get(), d_ptr->m_remote_credentials.at(remoteName));

    git_repository *repo = 0;
    git_clone_options opts = GIT_CLONE_OPTIONS_INIT;
    opts.fetch_opts.callbacks = remoteCallbacks.rawCallbacks();
    opts.checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE;
    qGitEnsureValue(0, git_clone(&repo, url.c_str(), internal::PathCodec::toLibGit2(path).data(), &opts));

    d_ptr->setData(repo);
}


void Repository::remoteAdd(const std::string& name, const std::string& url, bool changeUrlIfExists)
{
    git_remote *r = NULL;
    switch (git_remote_lookup(&r, SAFE_DATA, name.c_str())) {
    case GIT_ENOTFOUND:
        r = NULL;
        qGitThrow(git_remote_create(&r, SAFE_DATA, name.c_str(), url.c_str()));
        break;

    case GIT_OK:
        if (git_remote_url(r) != url) {
            if (changeUrlIfExists) {
                qGitThrow(git_remote_set_url(SAFE_DATA, name.c_str(), url.c_str()));
            } else {
                THROW("remote already exists");
            }
        }
        break;

    default:
        throw Exception();
        break;
    }
}


std::unique_ptr<Remote> Repository::remote(const std::string &remoteName) const
{
    git_remote *r = NULL;
    qGitThrow(git_remote_lookup(&r, SAFE_DATA, remoteName.c_str()));
    return std::make_unique<Remote>(r, d_ptr->m_remote_credentials.at(remoteName));
}


void Repository::fetch(const std::string& name, const std::string& head, const std::string &message)
{
    git_remote *_remote = NULL;
    qGitThrow(git_remote_lookup(&_remote, SAFE_DATA, name.c_str()));
    Remote remote(_remote, d_ptr->m_remote_credentials.at(name));
    remote.transferProgress = fetchProgress;

    using internal::StrArray;
    StrArray refs;
    if (!head.empty()) {
        const std::string refspec = "refs/heads/" + head + ":refs/remotes/" + name + '/' + head;
        std::vector<QByteArray> refspecs{internal::PathCodec::toLibGit2(refspec)};
        refs = StrArray(refspecs);
    }

    internal::RemoteCallbacks remoteCallbacks(d_ptr.get(), d_ptr->m_remote_credentials.at(name));
    git_fetch_options opts = GIT_FETCH_OPTIONS_INIT;
    opts.callbacks = remoteCallbacks.rawCallbacks();
    qGitThrow(git_remote_fetch(remote.data(), refs.count() > 0 ? &refs.data() : NULL, &opts, message.empty() ? NULL : message.c_str()));
}


std::vector<std::string> Repository::remoteBranches(const std::string& remoteName)
{
    git_remote *_remote = NULL;
    qGitThrow(git_remote_lookup(&_remote, SAFE_DATA, remoteName.c_str()));
    Remote remote(_remote, d_ptr->m_remote_credentials.at(remoteName));

    internal::RemoteCallbacks remoteCallbacks(d_ptr.get(), d_ptr->m_remote_credentials.at(remoteName));
    const auto callbacks = remoteCallbacks.rawCallbacks();
    qGitThrow(git_remote_connect(remote.data(), GIT_DIRECTION_FETCH, &callbacks, nullptr, nullptr));
    qGitEnsureValue(1, git_remote_connected(remote.data()));

    /* List the heads on the remote */
    const git_remote_head** remote_heads = NULL;
    size_t count = 0;
    qGitThrow(git_remote_ls(&remote_heads, &count, remote.data()));
    std::vector<std::string> heads;
    for (size_t i = 0; i < count; ++i) {
        const git_remote_head* head = remote_heads[i];
        if (head && head->name) {
            std::string ref = head->name;
            std::string_view prefix = "refs/heads/";
            if (starts_with(ref, prefix)) {
                heads.push_back(ref.substr(prefix.length()));
            }
        }
    }

    return heads;
}


void Repository::checkoutTree(const Object &treeish, const CheckoutOptions &opts)
{
    qGitThrow(git_checkout_tree(SAFE_DATA, treeish.constData(), opts.data()));
}


void Repository::checkoutHead(const CheckoutOptions &opts)
{
    qGitThrow(git_checkout_head(SAFE_DATA, opts.data()));
}


void Repository::checkoutRemote(const std::string& branch, const CheckoutOptions &opts, const std::string& remote)
{
    const std::string refspec = "refs/remotes/" + remote + "/" + branch;
    checkoutTree(lookupRevision(refspec), opts);

    qGitThrow(git_repository_set_head(SAFE_DATA, refspec.c_str()));
}


void Repository::reset(const Object &target, ResetType type)
{
    AVOID(target.isNull(), "can not reset to null target");

    git_reset_t resetType;
    switch (type) {
    case Soft:
        resetType = GIT_RESET_SOFT;
        break;
    case Mixed:
        resetType = GIT_RESET_MIXED;
        break;
    case Hard:
        resetType = GIT_RESET_HARD;
        break;
    default:
        THROW("invalid reset type argument");
    }

    qGitThrow(git_reset(SAFE_DATA, target.data(), resetType, NULL));
}

Rebase Repository::rebase(const Reference &branch, const Reference &upstream, const Reference &onto, const RebaseOptions &opts)
{
    git_rebase *rebase;
    internal::AnnotatedCommit commitBranch(*this, branch);
    internal::AnnotatedCommit commitUpstream(*this, upstream);
    internal::AnnotatedCommit commitOnto(*this, onto);
    qGitThrow(git_rebase_init(&rebase, data(), commitBranch.constData(), commitUpstream.constData(), commitOnto.constData(), opts.constData()));
    return Rebase(rebase);
}

bool Repository::shouldIgnore(const std::string &path) const
{
    auto usedPath = std::filesystem::path{path}.lexically_normal();
    if (usedPath.is_absolute()) {
        std::string wd(workDirPath());
        if (starts_with(usedPath.string(), wd)) {
            usedPath = usedPath.string().substr(wd.size());
        } else {
            THROW("Given path (" + path + ") is not within this repository's directory (" + wd + ").");
        }
    }

    int result;
    qGitThrow(git_status_should_ignore(&result, SAFE_DATA, usedPath.c_str()));
    return result;
}

void Repository::setIdentity(const Identity &id)
{
    const auto name = id.name.empty() ? nullptr : id.name.c_str();
    const auto email = id.email.empty() ? nullptr : id.email.c_str();
    qGitThrow(git_repository_set_ident(data(), name, email));
}

Repository::Identity Repository::identity() const
{
    const char *name;
    const char *email;
    qGitThrow(git_repository_ident(&name, &email, data()));
    return { name, email };
}

} // namespace LibGit2pp
