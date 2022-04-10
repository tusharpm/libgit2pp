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

#include "repository.h"
#include "config.h"
#include "remote.h"

using namespace LibGit2pp;


class TestPush : public TestBase
{
    Q_OBJECT

public:
    TestPush() :
        testdir(VALUE_TO_QSTR(TEST_DIR) + "/push_test/"),
        existingRepo(ExistingRepository + "/.git"),
        existingBareRepo(ExistingRepository + "_bare")
    {}

private slots:
    void initTestCase();
    void pushToNewTargetBranch();

private:
    const std::string testdir;
    const std::string existingRepo;
    const std::string existingBareRepo;

    void initBareLocalRepo();
};


void TestPush::initTestCase()
{
    TestBase::initTestCase();

    initBareLocalRepo();
}

void TestPush::initBareLocalRepo()
{
    removeDir(existingBareRepo);
    copyDir(existingRepo, existingBareRepo);

    Repository repo;
    repo.open(existingBareRepo);
    Config cfg = repo.configuration();
    cfg.setValue("core.bare", "true");
}


void TestPush::pushToNewTargetBranch()
{
    const std::string repoPath = testdir + "push_new_target_branch";
    QVERIFY(removeDir(repoPath));

    const std::string targetBranch("push_new_target_branch");

    Repository repo;
    try {
        repo.clone(existingBareRepo, repoPath);
        QScopedPointer<Remote> remote(repo.remote("origin"));
        remote->push(std::stringList("refs/heads/master:refs/heads/" + targetBranch));
    }
    catch (const LibGit2pp::Exception& ex) {
        QFAIL(ex.what());
    }

    QVERIFY(repo.remoteBranches("origin").contains(targetBranch));
}

QTEST_MAIN(TestPush)

#include "Push.moc"
