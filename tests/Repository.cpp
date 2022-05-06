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

using namespace LibGit2pp;

TEST_SUITE_BEGIN("Repository");

const std::string branchName = "new_branch";

TEST_CASE_FIXTURE(TestBase, "RemoteUrlChanging")
{
    Repository repo;
    auto testdir = getTestDir();
    repo.init(testdir);

    const std::string remoteName("origin");
    repo.remoteAdd(remoteName, HttpRemoteUrl);
    repo.remoteAdd(remoteName, GitRemoteUrl, true);

    auto remote = repo.remote(remoteName);
    CHECK(remote->url() == GitRemoteUrl);
}

TEST_CASE_FIXTURE(TestBase, "LookingUpRevision")
{
    Repository repo;
    repo.open(ExistingRepository);

    Object object = repo.lookupRevision("HEAD^{tree}");
    CHECK(Object::TreeType == object.type());
}

TEST_CASE_FIXTURE(TestBase, "CreateBranch")
{
    auto testdir = initTestRepo();
    Repository repo;
    repo.open(testdir);

    OId head = repo.head().target();
    CHECK(repo.createBranch(branchName).target() == head);
    CHECK(repo.lookupShorthandRef(branchName).target() == head);
}

TEST_CASE_FIXTURE(TestBase, "DeleteBranch")
{
    auto testdir = initTestRepo();
    Repository repo;
    repo.open(testdir);
    repo.createBranch(branchName);
    repo.deleteBranch(branchName);
    CHECK_THROWS_AS(repo.lookupShorthandRef(branchName), Exception);
}

TEST_CASE_FIXTURE(TestBase, "ShouldIgnore")
{
    auto testdir = initTestRepo();
    Repository repo;
    repo.open(testdir);

    const std::string ignoredFileName("Makefile");
    const std::string includedFileName("notignored.txt");

    // Relative paths
    CHECK(repo.shouldIgnore(ignoredFileName));
    CHECK(!repo.shouldIgnore(includedFileName));

    std::filesystem::path dir{testdir};
    auto testDirName = dir.filename();

    // Absolute paths
    CHECK(repo.shouldIgnore(dir / ignoredFileName));
    CHECK(!repo.shouldIgnore(dir / includedFileName));

    dir = dir.parent_path();

    // Path containing .. but still leading up to the repository
    CHECK(!repo.shouldIgnore(dir / ".." / dir.filename() / testDirName / includedFileName));

    // Absolute path outside the repository
    CHECK_THROWS_AS(repo.shouldIgnore(dir / ignoredFileName), Exception);
}

TEST_CASE_FIXTURE(TestBase, "IdentitySetting")
{
    auto testdir = getTestDir();
    Repository repo;
    repo.init(testdir);
    const Repository::Identity id{"name", "email"};
    repo.setIdentity(id);
    CHECK(repo.identity().name == id.name);
    CHECK(repo.identity().email == id.email);
}

TEST_SUITE_END();
