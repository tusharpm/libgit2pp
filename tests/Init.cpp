
#include "TestHelpers.h"
#include "doctest.h"
#include <filesystem>

using namespace LibGit2pp;

TEST_SUITE_BEGIN("Init");

TEST_CASE_FIXTURE(TestBase, "init")
{
    const std::string repoPath = getTestDir() + "/repo";

    CHECK(removeDir(repoPath));
    CHECK(!std::filesystem::exists(repoPath));
    std::filesystem::create_directories(repoPath);
    CHECK(std::filesystem::exists(repoPath));

    Repository repo;
    SUBCASE("not Bare")
    {
        try {
            repo.init(repoPath, false);
        } catch (const LibGit2pp::Exception& ex) {
            FAIL(ex.what());
        }

        CHECK(std::filesystem::is_regular_file(repoPath + "/.git/HEAD"));
    }

    SUBCASE("is Bare")
    {
        try {
            repo.init(repoPath, true);
        } catch (const LibGit2pp::Exception& ex) {
            FAIL(ex.what());
        }

        CHECK(!std::filesystem::exists(repoPath + "/.git"));
        CHECK(std::filesystem::is_regular_file(repoPath + "/HEAD"));
    }
}

TEST_SUITE_END();
