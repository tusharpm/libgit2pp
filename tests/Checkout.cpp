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

using namespace LibGit2pp;

void fetch(const std::string& branch, const std::string& repoPath, const std::string& remote = "origin")
{
    try {
        Repository repo;
        repo.init(repoPath);
        repo.remoteAdd(remote, HttpRemoteUrl);
        repo.fetch(remote, branch);
    }
    catch (const Exception& ex) {
        FAIL(ex.what());
    }
}

TEST_SUITE_BEGIN("Checkout");

TEST_CASE("Remote")
{
    fetch("master", testdir);

    try {
        Repository repo;
        repo.open(testdir);
        repo.checkoutRemote("master");
    }
    catch (const Exception& ex) {
        FAIL(ex.what());
    }
}

TEST_CASE("RemoteKde")
{
    fetch("master", testdir, "kde");

    try {
        Repository repo;
        repo.open(testdir);
        repo.checkoutRemote("master", CheckoutOptions(), "kde");
    }
    catch (const Exception& ex) {
        FAIL(ex.what());
    }
}


TEST_CASE("CommitAsTree")
{
    Repository repo;
    try {
        repo.clone(FileRepositoryUrl, testdir);
        OId id = OId::stringToOid("127c9e7d17");  // 127c9e7d17 is a commit where CMakeLists.txt was modified
        repo.checkoutTree(repo.lookupCommit(id), CheckoutOptions(CheckoutOptions::Safe, CheckoutOptions::RecreateMissing));
    } catch (const Exception& ex) {
        FAIL(ex.what());
    }

    StatusList status = repo.status(StatusOptions(StatusOptions::ShowOnlyIndex, StatusOptions::ExcludeSubmodules));
    bool found = false;
    for (size_t i = 0; i < status.entryCount(); ++i) {
        const StatusEntry entry = status.entryByIndex(i);
        if ((found = (entry.headToIndex().newFile().path() == "CMakeLists.txt"))) {
            break;
        }
    }
    QVERIFY2(found, "Expected path was not part of the checked out commit");
}

TEST_CASE("Head")
{
    const std::string fileName(testdir + "/CMakeLists.txt");

    Repository repo;
    try {
        repo.clone(FileRepositoryUrl, testdir);
        QVERIFY(QFile::remove(fileName));
        repo.checkoutHead(CheckoutOptions(CheckoutOptions::Force));
    } catch (const Exception& ex) {
        FAIL(ex.what());
    }

    QVERIFY(QFile::exists(fileName));
}

TEST_CASE("Paths")
{
    const std::stringList paths("CMakeLists.txt");
    Repository repo;
    try {
        repo.clone(FileRepositoryUrl, testdir);
        OId id = OId::stringToOid("7752cf5867");  // 7752cf5867 is a commit where many files were modified
        CheckoutOptions opts(CheckoutOptions::Safe, CheckoutOptions::RecreateMissing);
        opts.setPaths(paths);
        repo.checkoutTree(repo.lookupCommit(id), opts);
    } catch (const Exception& ex) {
        FAIL(ex.what());
    }

    StatusList status = repo.status(StatusOptions(StatusOptions::ShowOnlyIndex, StatusOptions::ExcludeSubmodules));
    std::stringList checkedoutPaths;
    for (size_t i = 0; i < status.entryCount(); ++i) {
        const StatusEntry entry = status.entryByIndex(i);
        checkedoutPaths << entry.headToIndex().newFile().path();
    }

    QCOMPARE(checkedoutPaths, paths);
}

TEST_SUITE_END();
