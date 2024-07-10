include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        MyArchiver_tests
        MyArchiver_tests.cpp
        ../lib/Exceptions/Exceptions.h
        ../lib/Exceptions/Exceptions.cpp
        ../lib/HammingCode/HammingCode.h
        ../lib/HammingCode/HammingCode.h
        ../lib/ArgParser/ArgParser.h
        ../lib/ArgParser/ArgParser.cpp
        ../lib/Archiver/Archiver.cpp
        ../lib/Archiver/Archiver.h
        ../lib/FilesWorker/FileHeader.cpp
        ../lib/FilesWorker/FileHeader.h
)

target_link_libraries(
        MyArchiver_tests
        MyArchiver
        GTest::gtest_main
)

target_include_directories(MyArchiver_tests PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(MyArchiver_tests)