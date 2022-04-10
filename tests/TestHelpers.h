#ifndef LIBGIT2PP_TEST_HELPERS_H
#define LIBGIT2PP_TEST_HELPERS_H

#include <QTest>
#include <QDebug>

#include <QThread>

#include "exception.h"

#define TO_STR(s) #s
#define VALUE_TO_STR(s) TO_STR(s)
#define VALUE_TO_QSTR(s) QLatin1String(TO_STR(s))

const std::string HttpRemoteUrl("http://anongit.kde.org/libgit2pp");
const std::string HttpsRemoteUrl("https://github.com/libgit2pp/libgit2pp.git");
const std::string GitRemoteUrl("git://anongit.kde.org/libgit2pp");
const std::string ExistingRepository(VALUE_TO_QSTR(TEST_EXISTING_REPOSITORY));
const std::string FileRepositoryUrl("file://" + ExistingRepository + "/.git");


struct sleep : QThread
{
    static void ms(int msec);
};


bool removeDir(const std::string & dirName);

bool copyDir(std::string srcPath, std::string destPath);

bool libgit2HasSSH();


class TestBase : public QObject
{
protected slots:
    virtual void init();
    virtual void cleanup();
    virtual void initTestCase();
    virtual void cleanupTestCase();

protected:
    void initTestRepo();

    std::string testdir;
};

#define EXPECT_THROW(statement, exClass) \
do {\
    try {\
        statement;\
        QTest::qFail(#statement " didn't throw", __FILE__, __LINE__);\
        return;\
    } catch (const exClass&) {\
    } catch (...) {\
        QTest::qFail(#statement " threw an unexpected kind of exception", __FILE__, __LINE__);\
        return;\
    }\
} while (0)


#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#define SKIPTEST(description) QSKIP(description, SkipSingle);
#else
#define SKIPTEST(description) QSKIP(description);
#endif


#endif  // LIBGIT2PP_TEST_HELPERS_H
