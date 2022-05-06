#include "TestHelpers.h"
#include <filesystem>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

using namespace LibGit2pp;

const std::string HttpRemoteUrl("http://anongit.kde.org/libgit2pp");
const std::string HttpsRemoteUrl("https://github.com/libgit2pp/libgit2pp.git");
const std::string GitRemoteUrl("git://anongit.kde.org/libgit2pp");
const std::string ExistingRepository(LIBGIT2PP_STR(TEST_EXISTING_REPOSITORY));
const std::string FileRepositoryUrl("file://" + ExistingRepository + "/.git");

void replaceSubstring(std::string& str, const std::string& match, const std::string& replacement)
{
    size_t index = 0;
    while (true) {
        // Locate the substring to replace.
        index = str.find(match, index);
        if (index == std::string::npos) break;

        // Make the replacement.
        str.replace(index, match.size(), replacement);

        // Advance index forward so the next iteration doesn't pick it up as well.
        index += replacement.size();
    }
}

bool removeDir(const std::string & dirName)
{
    return !std::filesystem::exists(dirName) || std::filesystem::remove_all(dirName);
}

bool copyDir(std::string srcPath, std::string destPath)
{
    std::filesystem::path srcDir{srcPath};
    if (!std::filesystem::exists(srcDir)) {
        FAIL("Source directory does not exist:", srcPath);
        return false;
    }

    std::filesystem::copy(srcDir, destPath, std::filesystem::copy_options::recursive);

    return true;
}

TestBase::TestBase() {
    initLibGit2pp();
}

TestBase::~TestBase() {
    shutdownLibGit2pp();
}

std::string TestBase::getTestDir(bool remove)
{
    auto testCase = doctest::getContextOptions()->currentTest;
    auto testCaseName = testCase->m_name;
    auto testSuiteName = testCase->m_test_suite;

    auto testdir = std::filesystem::path{LIBGIT2PP_STR(TEST_DIR)} / testSuiteName / testCaseName;
    if (remove) CHECK(removeDir(testdir));
    return testdir;
}

std::string TestBase::initTestRepo()
{
    try {
        auto testdir = getTestDir(false);
        Repository repo;
        repo.clone(FileRepositoryUrl, testdir);
        return testdir;
    } catch (const Exception& ex) {
        FAIL(ex.what());
    }
    return {};
}
