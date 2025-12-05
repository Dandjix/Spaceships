# Catch2 (assuming FetchContent or find_package is used above)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

include_directories(
        INTERFACE
        ${SDL3_INCLUDE_DIRS}
        ../src/.
        ../src/imported
)

# Collect test files
file(GLOB_RECURSE TEST_SOURCES *.cpp)

add_library(GTest::GTest INTERFACE IMPORTED
        LoadGame/LoadGameTests.cpp)

target_link_libraries(
        GTest::GTest
        INTERFACE
        SpaceshipsLib
    gtest_main
)

add_executable(SpaceshipsTests ${TEST_SOURCES})

target_link_libraries(
        SpaceshipsTests
        PRIVATE
        GTest::GTest
)

# Enable CLion / CTest integration
#include(CTest)
#include(Catch)
#catch_discover_tests(SpaceshipsTests)
