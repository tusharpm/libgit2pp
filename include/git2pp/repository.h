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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef LIBGIT2PP_REPOSITORY_H
#define LIBGIT2PP_REPOSITORY_H

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>

#include "libgit2pp_config.h"

#include "commit.h"
#include "database.h"
#include "object.h"
#include "ref.h"
#include "tree.h"
#include "index.h"
#include "statuslist.h"
#include "statusoptions.h"
#include "checkoutoptions.h"
#include "mergeoptions.h"
#include "cherrypickoptions.h"
#include "rebase.h"
#include "rebaseoptions.h"

namespace LibGit2pp
{
    class Commit;
    class Config;
    class Tag;
    class Tree;
    class Blob;
    class Signature;
    class Credentials;
    class Push;
    class Remote;
    class Diff;

    /**
     * @brief Wrapper class for git_repository.
     * Represents a Git repository.
     *
     * @ingroup LibGit2pp
     * @{
     */
    class LIBGIT2PP_EXPORT Repository
    {
        public:

            /**
             * Construct a wrapper around a libgit2 repository pointer.
             *
             * If `own` is true, this Repository takes ownership of the pointer, and makes
             * sure it is freed when the owner is deleted, unless there are other instances
             * sharing the ownership. If `own` is true, the pointer must not be freed manually,
             * and must not be passed to another Repository instance also with `own` true.
             */
            explicit Repository(git_repository *repository = 0, bool own = false);

            /**
             * Copy constructor; creates a copy of the object, sharing the same underlaying data
             * structure.
             */
            Repository(const Repository& other);

            /**
             * Destruct a previously allocated repository
             */
            ~Repository();

            /**
             * Look for a git repository and return its path. The lookup start from startPath and
             * walk across parent directories if nothing has been found. The lookup ends when the
             * first repository is found, or when reaching a directory that is referenced in
             * ceilingDirs, or when the filesystem changes (unless acrossFs is true).
             *
             * The method will automatically detect if the repository is bare (if there is a
             * repository).
             *
             * The function will only return successfully if a repository was found, otherwise an
             * exception is thrown, providing an error message.
             *
             * @param startPath
             * The base path where the lookup starts.
             *
             * @param acrossFs
             * If true, then the lookup will not stop when a filesystem device change is detected
             * while exploring parent directories.
             *
             * @param ceilingDirs
             * A list of absolute symbolic link free paths. The lookup will stop if any of these
             * paths are reached. Note that the lookup always performs on startPath no matter if
             * startPath appears in ceilingDirs.
             *
             * @return The path of the found repository
             * @throws LibGit2pp::Exception
             */
            static std::string discover(const std::string& startPath,
                                    bool acrossFs = false,
                                    const std::list<std::string>& ceilingDirs = {});

            /**
             * Constructs a new Git repository in the given folder.
             *
             * @param path the path to the repository
             * @param isBare if true, a Git repository without a working directory is created
             * at the pointed path. If false, provided path will be considered as the working
             * directory into which the .git directory will be created.
             *
             * @throws LibGit2pp::Exception
             */
            void init(const std::string& path, bool isBare = false);

            /**
             * Open a git repository.
             *
             * The 'path' argument must point to an existing git repository
             * folder, e.g.
             *
             * /path/to/my_repo/.git/ (normal repository)
             * objects/
             * index
             * HEAD
             *
             * /path/to/bare_repo/ (bare repository)
             * objects/
             * index
             * HEAD
             *
             * The method will automatically detect if 'path' is a normal
             * or bare repository or fail is 'path' is neither.
             *
             * @param path the path to the repository
             * @throws LibGit2pp::Exception
             */
            void open(const std::string& path);

            /**
             * Convenience function for finding and opening a git repository.
             *
             * Calls discover() with the given arguments, and passes the result to open().
             *
             * @throws LibGit2pp::Exception
             */
            void discoverAndOpen(const std::string &startPath,
                                 bool acrossFs = false,
                                 const std::list<std::string> &ceilingDirs = {});

            /**
             * Retrieve and resolve the reference pointed at by HEAD.
             *
             * @throws LibGit2pp::Exception
             */
            Reference head() const;

            /**
             * Check if a repository's HEAD is detached
             *
             * A repository's HEAD is detached when it points directly to a commit
             * instead of a branch.
             *
             * @throws LibGit2pp::Exception
             */
            bool isHeadDetached() const;

            /**
             * Check if the current branch is unborn
             *
             * An unborn branch is one named from HEAD but which doesn't exist
             * in the refs namespace, because it doesn't have any commit to point to.
             *
             * @throws LibGit2pp::Exception
             */
            bool isHeadUnborn() const;

            /**
             * Check if a repository is empty
             *
             * An empty repository has just been initialized and contains
             * no commits.
             *
             * @throws LibGit2pp::Exception
             */
            bool isEmpty() const;

            /**
             * Check if a repository is bare
             *
             * @throws LibGit2pp::Exception
             */
            bool isBare() const;

            /**
             * The name equals the repositories working directory name.
             * In case of a bare repository, the name equals the repositorie's directory.
             */
            std::string name() const;

            /**
             * Get the path to the repository
             */
            std::string path() const;

            /**
             * Get the path to the working directory
             */
            std::string workDirPath() const;

            /**
             * The repositories configuration file. Includes the global git configuration file.
             */
            Config configuration() const;

            /**
             * Lookup a reference by its name in a repository.
             *
             * @throws LibGit2pp::Exception
             * @return The reference with the given name
             */
            Reference lookupRef(const std::string& name) const;

            /**
             * Lookup a reference by its name in a repository and returns the oid of its target.
             *
             * @throws LibGit2pp::Exception
             * @return The OId of the target
             */
            OId lookupRefOId(const std::string& name) const;

            /**
             * Lookup a reference by its shorthand name in a repository.
             *
             * @throws LibGit2pp::Exception
             * @return The reference with the given name
             */
            Reference lookupShorthandRef(const std::string& shorthand) const;

            /**
             * Lookup a commit object from a repository.
             *
             * @throws LibGit2pp::Exception
             */
            Commit lookupCommit(const OId& oid) const;

            /**
             * Lookup a tag object from the repository.
             *
             * @throws LibGit2pp::Exception
             */
            Tag lookupTag(const OId& oid) const;

            /**
             * Lookup a tree object from the repository.
             *
             * @throws LibGit2pp::Exception
             */
            Tree lookupTree(const OId& oid) const;

            /**
             * Lookup a blob object from a repository.
             *
             * @throws LibGit2pp::Exception
             */
            Blob lookupBlob(const OId& oid) const;

            /**
             * Lookup a reference to one of the objects in a repostory.
             *
             * @throws LibGit2pp::Exception
             */
            Object lookupAny(const OId& oid) const;

            /**
             * @brief Lookup an \c Object by a revision specifier.
             *
             * See `man gitrevisions`, or
             * [the Git documentation](http://git-scm.com/docs/git-rev-parse.html#_specifying_revisions)
             * for information about the accepted syntax.
             * @param revspec the revision specifier.
             * @throws LibGit2pp::Exception
             * @return The found object if any.
             */
            Object lookupRevision(const std::string &revspec) const;

            /**
             * Create a new object id reference.
             *
             * The reference will be created in the repository and written
             * to the disk.
             *
             * @param name The name of the reference
             * @param oid The target of the reference
             * @param overwrite If there already exists a reference with the same name, whether to overwrite it
             * @param message The one line long message to be appended to the reflog
             * @throws LibGit2pp::Exception
             */
            Reference createRef(const std::string& name, const OId& oid, bool overwrite = true, const std::string &message = std::string());

            /**
             * Create a new symbolic reference.
             *
             * The reference will be created in the repository and written
             * to the disk.
             *
             * @param name The name of the reference
             * @param target The target of the reference
             * @param overwrite If there already exists a reference with the same name, whether to overwrite it
             * @param message The one line long message to be appended to the reflog
             * @throws LibGit2pp::Exception
             */
            Reference createSymbolicRef(const std::string& name, const std::string& target, bool overwrite = true, const std::string &message = std::string());

            /**
             * Create a new commit in the repository.
             *
             * @param tree The Tree to be used for the new Commit.
             * @param parents The parent Commits for the new Commit. Can be empty for the root (initial)
             *        Commit to the repository.
             * @param author Author signature.
             * @param committer Committer signature.
             * @param message The message for the new Commit.
             * @param ref Name of the reference that will be updated to point to this commit. If the
             *        reference is not direct, it will be resolved to a direct reference. Use "HEAD" to
             *        update the HEAD of the current branch and make it point to this commit. If the
             *        reference doesn't exist yet, it will be created. If it does exist, the first
             *        parent must be the tip of this branch.
             * @return The OId of the newly created commit.
             * @throws LibGit2pp::Exception
             */
            OId createCommit(const Tree& tree, const std::list<Commit>& parents, const Signature& author, const Signature& committer, const std::string& message, const std::string& ref = std::string());

            /**
             * Create a new lightweight tag pointing at a target object
             *
             * A new direct reference will be created pointing to
             * this target object. If `force` is true and a reference
             * already exists with the given name, it'll be replaced.
             *
             * @throws LibGit2pp::Exception
             */
            OId createTag(const std::string& name,
                              const Object& target,
                              bool overwrite = true);

            /**
             * Create a new tag in the repository from an object
             *
             * A new reference will also be created pointing to
             * this tag object. If `overwrite` is true and a reference
             * already exists with the given name, it'll be replaced.
             *
             * @throws LibGit2pp::Exception
             */
            OId createTag(const std::string& name,
                            const Object& target,
                            const Signature& tagger,
                            const std::string& message,
                            bool overwrite = true);

            /**
             * Delete an existing tag reference.
             *
             * @throws LibGit2pp::Exception
             */
            void deleteTag(const std::string& name);

            /**
             * Read a file from the working folder of a repository
             * and write it to the Object Database as a loose blob
             *
             * @throws LibGit2pp::Exception
             */
            OId createBlobFromFile(const std::string& path);

            /**
             * Write an in-memory buffer to the ODB as a blob
             *
             * @throws LibGit2pp::Exception
             */
            OId createBlobFromBuffer(const QByteArray& buffer);

            /**
             * Creates a new branch to this repository.
             * @param branchName The name of the new branch.
             * @param target The starting point of the branch, i.e. where the new branch will point to. Defaults to HEAD.
             * @param force Controls whether to overwrite an already existing branch.
             * @return A reference to the newly created branch
             * @throws LibGit2pp::Exception
             */
            Reference createBranch(const std::string &branchName, const Commit &target = Commit(), bool force = false);

            /**
             * Deletes a local branch.
             * @param branchName The name of the branch.
             * @throws LibGit2pp::Exception
             */
            void deleteBranch(const std::string &branchName);

            /**
             * Cherry-picks a commit.
             * @param commit The commit to cherry-pick.
             * @param opts The options specifying how the cherry-pick is made.
             * @throws LibGit2pp::Exception
             */
            void cherryPick(const Commit &commit, const CherryPickOptions &opts = CherryPickOptions());

            /**
             * Create a list with all the tags in the Repository
             * which name match a defined pattern
             *
             * If an empty pattern is provided, all the tags
             * will be returned.
             *
             * @param pattern Standard fnmatch pattern
             * @throws LibGit2pp::Exception
             */
            std::list<std::string> listTags(const std::string& pattern = {}) const;

            /**
             * Create a list with all references in the Repository.
             *
             * @param pattern Standard fnmatch pattern
             * @throws LibGit2pp::Exception
             */
            std::list<std::string> listReferences() const;

            /**
             * @brief Get the object database behind a Git repository
             *
             * @return a pointer to the object db
             */
            Database database() const;

            /**
             * @brief Get the Index file of a Git repository
             *
             * This is a cheap operation; the index is only opened on the first call,
             * and subsequent calls only retrieve the previous pointer.
             *
             * @throws LibGit2pp::Exception
             * @return The index file of the repository
             */
            Index index() const;

            /**
             * @brief Get the status information of the Git repository
             *
             * This function returns the status of the repository entries, according to
             * the given options.
             *
             * @throws LibGit2pp::Exception
             * @return The list of status entries
             */
            StatusList status(const StatusOptions &options) const;

            /**
             * How two nodes are related to each other in a graph.
             */
            struct GraphRelationship {
                /** How many steps one node is ahead of another. */
                size_t ahead;
                /** How many steps one node is behind of another. */
                size_t behind;
            };

            /**
             * Solves how the \a local commit is situated in the commit tree in relation to the
             * \a upstream commit. Both arguments can be any arbitrary commits in this repository
             * but it's useful to think them as the local branch tip commit and its corresponding
             * remote branch tip.
             *
             * @note The \a local commit can be both ahead of and behind from the \a upstream
             * commit if the branches have diverged.
             *
             * @param local The local commit.
             * @param upstream The upstream commit.
             * @return The relationship between the commits.
             * @throws LibGit2pp::Exception
             */
            GraphRelationship commitRelationship(const Commit &local, const Commit &upstream) const;

            /**
             * @brief Makes a Diff between two Trees.
             *
             * Either Tree argument can be a NULL Tree, but not both.
             *
             * @param oldTree the Tree on the `old' side of the diff.
             * @param newTree the Tree on the `new' side of the diff.
             * @throws LibGit2pp::Exception
             * @return The Diff between the provided Trees.
             */
            Diff diffTrees(const Tree &oldTree, const Tree &newTree) const;

            /**
             * Finds a merge base between two commits.
             * @param one The first Commit.
             * @param two The second Commit.
             * @return The merge base Commit.
             * @throws LibGit2pp::Exception
             */
            Commit mergeBase(const Commit &one, const Commit &two) const;

            /**
             * Merge two trees, producing an Index that reflects the result of the merge. The index
             * may be written as-is to the working directory or checked out. If the index is to be
             * converted to a tree, the caller should resolve any conflicts that arose as part of the merge.
             *
             * At least one of \a our or \a their must be a valid Tree.
             *
             * @param our The tree that reflects the destination tree.
             * @param their The tree to merge into the destination tree.
             * @param ancestor The common ancestor between the trees. May be null.
             * @param opts The merge options.
             * @return The merged index.
             */
            Index mergeTrees(const Tree &our, const Tree &their, const Tree &ancestor = Tree(), const MergeOptions &opts = MergeOptions());

            /**
             * @brief Sets a \c Credentials object to be used for a named remote.
             *
             * Some remotes require authentication when communicating with them. Authentication
             * is performed by using \c Credentials objects. Each named remote can have its own
             * \c Credentials object. The credentials for a remote must be set using this method
             * before trying to communicate with it.
             *
             * @param remoteName the name of the remote
             * @param credentials the \c Credentials to be used for the remote
             */
            void setRemoteCredentials(const std::string& remoteName, Credentials credentials);

            /**
            * Clone a git repository.
            *
            * Signal cloneProgress(int) is emitted with progress in percent.
            *
            * @param url URL of the git repository
            * @param path non-existing directory for the new clone
            * @throws LibGit2pp::Exception
            */
            void clone(const std::string& url, const std::string& path);

            /**
            * Add remote repository.
            *
            * @param name name of the remote
            * @param url URL of the remote git repository
            * @param changeUrlIfExists affects the behaviour if the remote already exists:
            * if true changes the remote's URL, if false throws an exception.
            * @throws LibGit2pp::Exception
            */
            void remoteAdd(const std::string& name, const std::string& url, bool changeUrlIfExists = false);

            /**
             * Gets a named remote from this repository. The caller is responsible for
             * managing the returned object.
             * @param remoteName the name of the remote
             * @throws LibGit2pp::Exception
             */
            std::unique_ptr<Remote> remote(const std::string &remoteName) const;

            /**
            * Fetch from known remote repository.
            *
            * @param remote Name of the remote repository (e.g. "origin")
            * @param head Name of head to fetch (e.g. "master"). If left as the default
            *        the fetch heads configured for the remote are used.
            * @param message The message to insert into the reflogs. If left as the
            *        default (a null string), a message "fetch <name>" is used , where <name>
            *        is the name of the remote (or its url, for in-memory remotes).
            * @throws LibGit2pp::Exception
            */
            void fetch(const std::string& remote, const std::string& head = std::string(), const std::string &message = std::string());

            std::list<std::string> remoteBranches(const std::string& remoteName);


            /**
             * Checkout a treeish, i.e. a Commit, Tag or Tree.
             * @param treeish the Object to checkout
             * @param opts the options used in the checkout
             * @throws LibGit2pp::Exception
             */
            void checkoutTree(const Object &treeish, const CheckoutOptions &opts = CheckoutOptions());

            /**
             * Updates files in the index and the working tree to match the content
             * of the commit pointed at by HEAD.
             * @param opts the options used in the checkout
             * @throws LibGit2pp::Exception
             */
            void checkoutHead(const CheckoutOptions &opts = CheckoutOptions());

            /**
            * Checkout a remote branch without creating a local branch.
            *
            * @param branch  branch name
            * @param opts the options used in the checkout
            * @param remote  remote which should be used, default is 'origin'
            * @throws LibGit2pp::Exception
            */
            void checkoutRemote(const std::string& branch, const CheckoutOptions &opts = CheckoutOptions(), const std::string& remote = "origin");

            /**
             * Types of reset operation.
             */
            enum ResetType {
                Soft,      ///< Move the head to the given commit
                Mixed,     ///< Soft plus reset index to the commit
                Hard       ///< Mixed plus changes in working tree discarded
            };

            /**
             * Sets the current head to the specified \a target oid and optionally
             * resets the index and working tree to match.
             *
             * @param target the committish to which the HEAD should be moved to
             * @param type what kind of reset should be performed
             * @throws LibGit2pp::Exception
             */
            void reset(const Object &target, ResetType type = Mixed);

            /**
             * Initializes a rebase object for rebasing the changes in \a branch
             * relative to \a upstream onto another branch.
             *
             * @param branch The terminal commit to rebase
             * @param upstream The commit to begin rebasing from, or \c null to rebase all
             *                 reachable commits
             * @param onto The branch to rebase onto, or \c null to rebase onto the given
             *             upstream
             * @param opts Options to specify how rebase is performed
             * @return The initialized rebase object
             * @throws LibGit2pp::Exception
             */
            Rebase rebase(const Reference &branch, const Reference &upstream, const Reference &onto, const RebaseOptions &opts);

            /**
             * Checks if the repository's ignore rules would ignore the given \a path.
             *
             * If \a path is relative it is considered to be relative to the Repository's
             * working directory. If \a path is absolute it must point to a location
             * within this Repository's working directory or an exception is thrown.
             *
             * @throws LibGit2pp::Exception
             */
            bool shouldIgnore(const std::string &path) const;

            struct Identity {
                std::string name;
                std::string email;
            };

            /**
             * Set the identity to be used for writing reflogs.
             *
             * @param id The name and email to use for the reflog entries
             *
             * If both \c name and \c email are set in the given \a id, then \a id will be
             * used to write to the reflog. Set either \c name or \c email to empty string
             * to unset. When unset, the identity will be taken from the repository's
             * configuration.
             *
             * @throws LibGit2pp::Exception
             */
            void setIdentity(const Identity &id);

            /**
             * Retrieve the configured identity to use for reflogs.
             *
             * @throws LibGit2pp::Exception
             */
            Identity identity() const;

            git_repository* data() const;
            const git_repository* constData() const;

            std::function<void(int)> cloneProgress;
            std::function<void(int)> fetchProgress;

        private:
            class Private;
            std::shared_ptr<Private> d_ptr;
    };

    /**@}*/
}

#endif // LIBGIT2PP_REPOSITORY_H
