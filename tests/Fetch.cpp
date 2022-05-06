/******************************************************************************
* Copyright (C) 2014 Peter K�mmel <syntheticpp@gmx.net>
*
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

#include <iostream>
#include <thread>

using namespace LibGit2pp;

TEST_SUITE_BEGIN("Fetch");

struct FetchTest : TestBase {
    void fetch(const std::string& branch, const std::string dirname)
    {
        const auto testdir = getTestDir();
        const std::string repoPath = testdir + "/fetch_test/" + dirname;

        CHECK(removeDir(repoPath));
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(500ms);

        try {
            Repository repo;
            repo.init(repoPath);
            repo.remoteAdd("kde", HttpRemoteUrl);
            repo.fetch("kde", branch);
        }
        catch (const Exception& ex) {
            FAIL(ex.what());
        }
    }
};

TEST_CASE_FIXTURE(FetchTest, "remoteAdd")
{
    const auto testdir = getTestDir();
    const std::string repoPath = testdir + "/fetch_test/remote_add";

    CHECK(removeDir(repoPath));
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(500ms);

    try {
        Repository repo;
        repo.init(repoPath);
        repo.remoteAdd("kde", HttpRemoteUrl);
    }
    catch (const Exception& ex) {
        FAIL(ex.what());
    }
}


TEST_CASE_FIXTURE(FetchTest, "remoteAddExisiting")
{
    const auto testdir = getTestDir();
    const std::string repoPath = testdir + "/fetch_test/add_exisiting";

    CHECK(removeDir(repoPath));
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(500ms);

    try {
        Repository repo;
        repo.init(repoPath);
        repo.remoteAdd("kde", HttpRemoteUrl);
        repo.remoteAdd("kde", HttpRemoteUrl);
    }
    catch (const Exception& ex) {
        FAIL(ex.what());
    }

    // TODO verify remote branches with unit test
}

TEST_CASE_FIXTURE(FetchTest, "remoteAddExisitingDifferentUrl")
{
    const auto testdir = getTestDir();
    const std::string repoPath = testdir + "/fetch_test/add_existing_url";

    CHECK(removeDir(repoPath));
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(500ms);

    Repository repo;
    try {
        repo.init(repoPath);
        repo.remoteAdd("kde", HttpRemoteUrl);
    }
    catch (const Exception& ex) {
        FAIL(ex.what());
    }

    CHECK_THROWS_AS_MESSAGE(repo.remoteAdd("kde", "XYZ"), Exception, "add invalid remote URL");
}

TEST_CASE_FIXTURE(FetchTest, "fetchMaster")
{
    fetch("master", "fetch_master");
}

TEST_CASE_FIXTURE(FetchTest, "fetchAll")
{
    fetch("", "fetch_default");
}

TEST_CASE_FIXTURE(FetchTest, "fetchAdditionalBranch")
{
    fetch("master", "fetch_additional");
    const auto testdir = getTestDir(false);
    const std::string repoPath = testdir + "/fetch_test/fetch_additional";

    try {
        Repository repo;
        repo.open(repoPath);
        repo.fetch("kde", "develop");
    }
    catch (const Exception& ex) {
        FAIL(ex.what());
    }

    // TODO verify remote branches with unit test
}

TEST_CASE_FIXTURE(FetchTest, "remoteBranches")
{
    const auto testdir = getTestDir();
    const std::string repoPath = testdir + "/fetch_test/remote_branches";

    CHECK(removeDir(repoPath));
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(500ms);

    try {
        Repository repo;
        repo.init(repoPath);
        repo.remoteAdd("kde", HttpRemoteUrl);
        auto heads = repo.remoteBranches("kde");
        MESSAGE("heads: ", heads);
        CHECK(heads.size() > 1);
        CHECK(std::find(heads.begin(), heads.end(), "master") != heads.end());
    }
    catch (const Exception& ex) {
        FAIL(ex.what());
    }
}

TEST_SUITE_END();
