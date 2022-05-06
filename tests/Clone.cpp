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

#include <bitset>
#include <chrono>
#include <iostream>
#include <thread>

using namespace LibGit2pp;

TEST_SUITE_BEGIN("Checkout");

struct CloneTest : TestBase {
    void clone(const std::string& url, const Credentials &credentials = {})
    {
        int clone_progress = 0;

        Repository repo;
        repo.setRemoteCredentials("origin", credentials);
        repo.cloneProgress = [&clone_progress](int percent){
            clone_progress = percent;
            if (percent % 20 == 0) {
                MESSAGE("Progress: ", percent);
            }
        };

        std::string dirname = url;
        replaceSubstring(dirname, ":", "");
        replaceSubstring(dirname, "//", "/");
        replaceSubstring(dirname, "/", "_");
        replaceSubstring(dirname, ".", "_");
        const auto testdir = getTestDir();
        const std::string repoPath = testdir + "/clone_test/" + dirname;

        removeDir(repoPath);
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(500ms);
        clone_progress = 0;

        MESSAGE("Cloning ", url);
        try {
            repo.clone(url, repoPath);
        }
        catch (const Exception& ex) {
            FAIL(ex.what());
        }

        CHECK(clone_progress == 100);
    }
};

TEST_CASE_FIXTURE(CloneTest, "fileProtocol")
{
    clone(FileRepositoryUrl);
}


TEST_CASE_FIXTURE(CloneTest, "gitProtocol")
{
    clone(GitRemoteUrl);
}


TEST_CASE_FIXTURE(CloneTest, "httpProtocol")
{
    clone(HttpRemoteUrl);
}


TEST_CASE_FIXTURE(CloneTest, "httpsProtocol")
{
    clone(HttpsRemoteUrl);
}

TEST_SUITE_END();
