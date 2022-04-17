cmake_minimum_required(VERSION 3.12)

set(testdir ${CMAKE_BINARY_DIR}/TestDir)
set(existing_repository ${testdir}/existing_repository)
if(NOT EXISTS ${existing_repository})
    message(STATUS "Creating test repository ...")
    file(MAKE_DIRECTORY ${existing_repository}/.git)
    file(GLOB_RECURSE gitfiles RELATIVE ${CMAKE_SOURCE_DIR}/.git ${CMAKE_SOURCE_DIR}/.git/*)
    foreach(gitfile ${gitfiles})
        configure_file(${CMAKE_SOURCE_DIR}/.git/${gitfile} ${existing_repository}/.git/${gitfile} COPYONLY)
    endforeach()
endif()
message(STATUS "Using test repository at '${existing_repository}'")

set(DOCTEST_DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR})
file(DOWNLOAD
    https://raw.githubusercontent.com/onqtam/doctest/v2.4.8/doctest/doctest.h
    ${DOCTEST_DOWNLOAD_DIR}/doctest.h
    EXPECTED_HASH SHA1=a1ff6eef4f235bca313c1fc5d0bc86126f2ed79a
)

add_library(TestHelpers OBJECT TestHelpers.cpp TestHelpers.h)
target_compile_definitions(TestHelpers PRIVATE TEST_DIR=${testdir} TEST_EXISTING_REPOSITORY=${existing_repository})
target_include_directories(TestHelpers PUBLIC ${DOCTEST_DOWNLOAD_DIR})
target_link_libraries(TestHelpers PUBLIC git2pp git2)

macro(addTest name)
    set(testname test${name})
    add_executable(${testname} ${name}.cpp)
    target_link_libraries(${testname} TestHelpers)
    add_test(NAME ${testname} COMMAND ${testname})
endmacro()

addTest(Checkout)
addTest(Clone)
addTest(Diff)
addTest(Fetch)
addTest(Init)
addTest(Push)
addTest(Rebase)
addTest(Repository)
addTest(Revision)
