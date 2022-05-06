#ifndef LIBGIT2PP_TEST_HELPERS_H
#define LIBGIT2PP_TEST_HELPERS_H

#include "git2pp.h"

extern const std::string HttpRemoteUrl;
extern const std::string HttpsRemoteUrl;
extern const std::string GitRemoteUrl;
extern const std::string ExistingRepository;
extern const std::string FileRepositoryUrl;

void replaceSubstring(std::string& source, const std::string& match, const std::string& replacement);
bool removeDir(const std::string & dirName);
bool copyDir(std::string srcPath, std::string destPath);

struct TestBase
{
    TestBase();
    ~TestBase();

    std::string getTestDir(bool remove = true);
    std::string initTestRepo(); //< returns testDir used by repo.
};
#endif  // LIBGIT2PP_TEST_HELPERS_H
