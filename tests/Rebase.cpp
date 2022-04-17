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

using namespace LibGit2pp;

TEST_SUITE_BEGIN("Rebase");

class TestRebase : public TestBase
{
public:
    TestRebase();

private slots:
    void init();
    void cleanup();

    void TestRebasingMasterOntoAnotherBranchProducesCorrectTopology();

private:
    std::shared_ptr<Repository> repo;
    Signature sig;

    OId commitIndexToRef(const std::string &refSpec);
    void writeToIndex(const std::string &path, const std::string &text);
};

TestRebase::TestRebase()
    : TestBase(),
      sig("unknown", "unknown")
{
}

void TestRebase::init()
{
    initTestRepo();
    repo = std::shared_ptr<Repository>(new Repository);
    repo->open(testdir + "/.git");
}

void TestRebase::cleanup()
{
    repo.clear();
    TestBase::cleanup();
}

OId TestRebase::commitIndexToRef(const std::string &refSpec)
{
    Tree index = repo->lookupTree(repo->index().createTree());
    std::list<Commit> parents;
    parents.append(repo->lookupCommit(repo->head().target()));
    return repo->createCommit(index, parents, sig, sig, "commit", refSpec);
}

void TestRebase::writeToIndex(const std::string &path, const std::string &text)
{
    QFile file(path);
    file.open(QFile::ReadWrite);
    file.write(text.toUtf8());
    file.close();
    repo->index().addByPath(QFileInfo(path).fileName());
    repo->index().write();
}

void TestRebase::TestRebasingMasterOntoAnotherBranchProducesCorrectTopology()
{
    // set up the repository
    std::string refSpecOntoBranch("refs/heads/onto");
    repo->createBranch("onto");
    std::string pathOntoBranchFile(testdir + "/onto.txt");
    writeToIndex(pathOntoBranchFile, "testing");
    OId oidOntoCommit = commitIndexToRef(refSpecOntoBranch);

    repo->checkoutHead(CheckoutOptions(CheckoutOptions::Force));

    std::string pathMasterBranchFile(testdir + "/master.txt");
    writeToIndex(pathMasterBranchFile, "testing testing");
    commitIndexToRef("HEAD");

    // rebase master onto another branch
    Reference refOnto = repo->lookupRef(refSpecOntoBranch);
    Reference refMaster = repo->lookupRef("refs/heads/master");
    Reference refUpstream = repo->lookupRef("refs/remotes/origin/master");
    Rebase rebase = repo->rebase(refMaster, refUpstream, refOnto, RebaseOptions(CheckoutOptions(CheckoutOptions::Safe, CheckoutOptions::RecreateMissing)));
    rebase.next();
    OId oidRebasedMaster = rebase.commit(sig, sig, std::string());
    rebase.finish(sig);

    // check results
    RevWalk walk(*repo);
    walk.setSorting(RevWalk::Topological);
    walk.pushHead();
    OId oid;
    QVERIFY(walk.next(oid));
    QCOMPARE(oidRebasedMaster, oid);
    QVERIFY(walk.next(oid));
    QCOMPARE(oidOntoCommit, oid);
    QVERIFY(walk.next(oid));
    QCOMPARE(refUpstream.target(), oid);
}

TEST_SUITE_END();
