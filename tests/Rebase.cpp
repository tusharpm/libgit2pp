/******************************************************************************
* Permission to use, copy, modify, and distribute the software
* and its documentation for any purpose and without fee is hereby
* granted, provided that the above copyright notice appear in all
* copies and that both that the copyright notice and this
* permission notice and warranty disclaimer appear in supporting
* documentation, and that the name of the author not be used in
* advertising or publicity pertaining to distribution of the
* software without specific, written prior permission.
*
* The author disclaim all warranties with regard to this
* software, including all implied warranties of merchantability
* and fitness.  In no event shall the author be liable for any
* special, indirect or consequential damages or any damages
* whatsoever resulting from loss of use, data or profits, whether
* in an action of contract, negligence or other tortious action,
* arising out of or in connection with the use or performance of
* this software.
*/

#include "TestHelpers.h"
#include "doctest.h"
#include <filesystem>
#include <fstream>

using namespace LibGit2pp;

TEST_SUITE_BEGIN("Rebase");

Signature sig{"unknown", "unknown"};

OId commitIndexToRef(Repository& repo, const std::string &refSpec)
{
    Tree index = repo.lookupTree(repo.index().createTree());
    std::list<Commit> parents{repo.lookupCommit(repo.head().target())};
    return repo.createCommit(index, parents, sig, sig, "commit", refSpec);
}

void writeToIndex(Repository& repo, const std::string &path, const std::string &text)
{
    std::ofstream{path} << text;
    repo.index().addByPath(std::filesystem::path{path}.filename());
    repo.index().write();
}

TEST_CASE_FIXTURE(TestBase, "Rebase master onto another branch produces correct topology")
{
    // set up the repository
    const auto testdir = getTestDir();
    Repository repo;
    repo.open(testdir + "/.git");

    std::string refSpecOntoBranch("refs/heads/onto");
    repo.createBranch("onto");
    std::string pathOntoBranchFile(testdir + "/onto.txt");
    writeToIndex(repo, pathOntoBranchFile, "testing");
    OId oidOntoCommit = commitIndexToRef(repo, refSpecOntoBranch);

    repo.checkoutHead(CheckoutOptions(CheckoutOptions::Force));

    std::string pathMasterBranchFile(testdir + "/master.txt");
    writeToIndex(repo, pathMasterBranchFile, "testing testing");
    commitIndexToRef(repo, "HEAD");

    // rebase master onto another branch
    Reference refOnto = repo.lookupRef(refSpecOntoBranch);
    Reference refMaster = repo.lookupRef("refs/heads/master");
    Reference refUpstream = repo.lookupRef("refs/remotes/origin/master");
    Rebase rebase = repo.rebase(refMaster, refUpstream, refOnto, RebaseOptions(CheckoutOptions(CheckoutOptions::Safe, CheckoutOptions::RecreateMissing)));
    rebase.next();
    OId oidRebasedMaster = rebase.commit(sig, sig, std::string());
    rebase.finish(sig);

    // check results
    RevWalk walk{repo};
    walk.setSorting(RevWalk::Topological);
    walk.pushHead();
    OId oid;
    CHECK(walk.next(oid));
    CHECK_EQ(oidRebasedMaster, oid);
    CHECK(walk.next(oid));
    CHECK_EQ(oidOntoCommit, oid);
    CHECK(walk.next(oid));
    CHECK_EQ(refUpstream.target(), oid);
}

TEST_SUITE_END();
