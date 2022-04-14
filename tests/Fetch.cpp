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
#include <bitset>

using namespace LibGit2pp;

void fetch(const std::string& branch, const std::string dirname)
{
    LibGit2pp::Repository repo;

    const std::string repoPath = testdir + "/fetch_test/" + dirname;

    QVERIFY(removeDir(repoPath));
    sleep::ms(500);

    try {
        repo.init(repoPath);
        repo.remoteAdd("kde", HttpRemoteUrl);
        repo.fetch("kde", branch);
    }
    catch (const LibGit2pp::Exception& ex) {
        FAIL(ex.what());
    }
}

TEST_SUITE_BEGIN("Fetch");

TEST_CASE("remoteAdd")
{
    LibGit2pp::Repository repo;

    const std::string repoPath = testdir + "/fetch_test/remote_add";

    QVERIFY(removeDir(repoPath));
    sleep::ms(500);
    m_progress = 0;

    try {
        repo.init(repoPath);
        repo.remoteAdd("kde", HttpRemoteUrl);
    }
    catch (const LibGit2pp::Exception& ex) {
        FAIL(ex.what());
    }
}


TEST_CASE("remoteAddExisiting")
{
    LibGit2pp::Repository repo;

    const std::string repoPath = testdir + "/fetch_test/add_exisiting";

    QVERIFY(removeDir(repoPath));
    sleep::ms(500);

    try {
        repo.init(repoPath);
        repo.remoteAdd("kde", HttpRemoteUrl);
        repo.remoteAdd("kde", HttpRemoteUrl);
    }
    catch (const LibGit2pp::Exception& ex) {
        FAIL(ex.what());
    }

    // TODO verify remote branches with unit test
}



TEST_CASE("remoteAddExisitingDifferentUrl")
{
    LibGit2pp::Repository repo;

    const std::string repoPath = testdir + "/fetch_test/add_existing_url";

    QVERIFY(removeDir(repoPath));
    sleep::ms(500);

    try {
        repo.init(repoPath);
        repo.remoteAdd("kde", HttpRemoteUrl);
    }
    catch (const LibGit2pp::Exception& ex) {
        FAIL(ex.what());
    }

    try {
        repo.remoteAdd("kde", "XYZ");
    }
    catch (const LibGit2pp::Exception&) {
        return;
    }

    FAIL("Could add invalid remote URL");
}

TEST_CASE("fetchMaster")
{
    fetch("master", "fetch_master");
}

TEST_CASE("fetchAll")
{
    fetch("", "fetch_default");
}

TEST_CASE("fetchAdditionalBranch")
{
    fetch("master", "fetch_additional");

    LibGit2pp::Repository repo;

    const std::string repoPath = testdir + "/fetch_test/fetch_additional";

    try {
        repo.open(repoPath);
        repo.fetch("kde", "develop");
    }
    catch (const LibGit2pp::Exception& ex) {
        FAIL(ex.what());
    }

    // TODO verify remote branches with unit test
}

TEST_CASE("remoteBranches")
{
    LibGit2pp::Repository repo;

    const std::string repoPath = testdir + "/fetch_test/remote_branches";

    QVERIFY(removeDir(repoPath));
    sleep::ms(500);

    std::stringList heads;
    try {
        repo.init(repoPath);
        repo.remoteAdd("kde", HttpRemoteUrl);
        heads = repo.remoteBranches("kde");
    }
    catch (const LibGit2pp::Exception& ex) {
        FAIL(ex.what());
    }

    qDebug() << "heads" << heads;

    QVERIFY(!heads.isEmpty());
    QVERIFY(heads.size() > 1);
    QVERIFY(heads.contains("master"));
}

TEST_SUITE_END();
