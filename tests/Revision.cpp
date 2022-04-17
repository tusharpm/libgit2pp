// A try to port libgit2 status.c example to libgit2pp


#include "TestHelpers.h"
#include "doctest.h"

#include <iostream>
#include <bitset>

using namespace LibGit2pp;

TEST_SUITE_BEGIN("Revision");

class TestRevision : public TestBase
{
private slots:
    void init();
    void cleanup();

    void revwalk();

private:
    QPointer<Repository> repo;
};

void TestRevision::init()
{
    QVERIFY(!repo);

    // Create a new repository object
    repo = new Repository();

    QVERIFY(repo);

    try {
        // Open a local fixed path
        repo->open(ExistingRepository);
    } catch (const Exception& ex) {
        FAIL(ex.what());
    }
}

void TestRevision::cleanup()
{
    QVERIFY(repo);
    delete repo;
    QVERIFY(!repo);

    TestBase::cleanup();
}

void TestRevision::revwalk()
{
    try {

        RevWalk rw(*repo);

        rw.setSorting(RevWalk::Topological);

        rw.pushHead();

        Commit commit;
        while(rw.next(commit)) {
            QByteArray qb = commit.oid().format();
            std::cout << qb.data() << std::endl;
        }

    } catch (const Exception& ex) {
        FAIL(ex.what());
    }
}

TEST_SUITE_END();
