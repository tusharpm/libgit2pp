#ifndef LIBGIT2PP_TEST_HELPERS_H
#define LIBGIT2PP_TEST_HELPERS_H

#include "git2pp.h"

const std::string HttpRemoteUrl("http://anongit.kde.org/libgit2pp");
const std::string HttpsRemoteUrl("https://github.com/libgit2pp/libgit2pp.git");
const std::string GitRemoteUrl("git://anongit.kde.org/libgit2pp");
const std::string ExistingRepository(LIBGIT2PP_STR(TEST_EXISTING_REPOSITORY));
const std::string FileRepositoryUrl("file://" + ExistingRepository + "/.git");

bool removeDir(const std::string & dirName);

bool copyDir(std::string srcPath, std::string destPath);

bool libgit2HasSSH();


class TestBase : public QObject
{
protected slots:
    virtual void initTestCase();
    virtual void cleanupTestCase();

protected:
    void initTestRepo();
};

#define EXPECT_THROW(statement, exClass) \
do {\
    try {\
        statement;\
        QTest::FAIL(#statement " didn't throw", __FILE__, __LINE__);\
        return;\
    } catch (const exClass&) {\
    } catch (...) {\
        QTest::FAIL(#statement " threw an unexpected kind of exception", __FILE__, __LINE__);\
        return;\
    }\
} while (0)

#define SKIPTEST(description) QSKIP(description);

#endif  // LIBGIT2PP_TEST_HELPERS_H
