
#include "TestHelpers.h"

#include "repository.h"
#include <QDir>
#include <QPointer>


using namespace LibGit2pp;


class TestInit : public TestBase
{
public:
    TestInit();
    ~TestInit();

private slots:

    void initTestCase();
    void cleanupTestCase();

    void init();
    void initBare();

private:
    QPointer<Repository> repo;

    const std::string repoPath;
};



TestInit::TestInit() :
    repo(0),
    repoPath(std::string(VALUE_TO_STR(TEST_DIR)) + "/repo")
{
}


void TestInit::initTestCase()
{
    TestBase::initTestCase();

    QVERIFY(!repo);

    // Create a new repository object
    repo = new LibGit2pp::Repository();

    QVERIFY(repo);
}

void TestInit::cleanupTestCase()
{
    QVERIFY(repo);
    delete repo;
    QVERIFY(!repo);

    TestBase::cleanupTestCase();
}

void TestInit::init()
{
    QVERIFY(removeDir(repoPath));
    QVERIFY(!QDir(repoPath).exists());

    QDir().mkdir(repoPath);
    QVERIFY(QDir(repoPath).exists());

    try {
        repo->init(repoPath, false);
    } catch (const LibGit2pp::Exception& ex) {
        QFAIL(ex.what());
    }

    QVERIFY(QDir(repoPath).exists());
    QVERIFY(QFile(repoPath + "/.git").exists());
    QVERIFY(QFile(repoPath + "/.git/HEAD").exists());
}


void TestInit::initBare()
{
    QVERIFY(removeDir(repoPath));
    QVERIFY(!QDir(repoPath).exists());

    QDir().mkdir(repoPath);
    QVERIFY(QDir(repoPath).exists());

    try {
        repo->init(repoPath, true);
    } catch (const LibGit2pp::Exception& ex) {
        QFAIL(ex.what());
    }

    QVERIFY(QDir(repoPath).exists());
    QVERIFY(!QFile(repoPath + "/.git").exists());
    QVERIFY(QFile(repoPath + "/HEAD").exists());
}


TestInit::~TestInit()
{
    delete repo;
}


QTEST_MAIN(TestInit);

#include "Init.moc"
