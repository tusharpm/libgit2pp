#ifndef LIBGIT2PP_TEST_HELPERS_H
#define LIBGIT2PP_TEST_HELPERS_H

#include "git2pp.h"

extern const std::string HttpRemoteUrl;
extern const std::string HttpsRemoteUrl;
extern const std::string GitRemoteUrl;
extern const std::string ExistingRepository;
extern const std::string FileRepositoryUrl;

std::string getTestDir(bool remove = true);

bool removeDir(const std::string & dirName);

bool copyDir(std::string srcPath, std::string destPath);

bool libgit2HasSSH();


struct TestBase
{
    virtual void initTestCase();
    virtual void cleanupTestCase();

    void initTestRepo();
};

#endif  // LIBGIT2PP_TEST_HELPERS_H
