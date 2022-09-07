#include "sodium/nac/io/file_io.h"

#include <cstdio>
#include <filesystem>

#include <gtest/gtest.h>

#include "sodium/nac/exceptions/io_exception.h"

// operator / is overloaded to concatenate file path in std::filesystem
static const std::filesystem::path TEMP_DIRECTORY_PATH = std::filesystem::temp_directory_path() / "sodium_test_files";
static const std::filesystem::path EMPTY_FILE_PATH = TEMP_DIRECTORY_PATH / "empty_file.txt";
static const std::filesystem::path TEST_FILE_PATH = TEMP_DIRECTORY_PATH / "test_file.txt";
static const std::string TEST_FILE_TEXT("Text\n    File used\n\tfor\t\t\nunit testing\n\t  File\nReader\n");

class FileReaderTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        std::filesystem::create_directory(TEMP_DIRECTORY_PATH);

        std::unique_ptr<std::FILE, decltype(&std::fclose)> empty_file(std::fopen(EMPTY_FILE_PATH.c_str(), "w"),
                                                                      &std::fclose);
        if (!empty_file) {
            FAIL() << "FileReaderTest: error creating temporary file: " << EMPTY_FILE_PATH << '\n';
        }

        std::unique_ptr<std::FILE, decltype(&std::fclose)> temp_file(std::fopen(TEST_FILE_PATH.c_str(), "w"),
                                                                     &std::fclose);
        if (!temp_file) {
            FAIL() << "FileReaderTest: error creating temporary file: " << TEST_FILE_PATH << '\n';
        }

        std::fputs(TEST_FILE_TEXT.c_str(), temp_file.get());
    }

    static void TearDownTestSuite() {
        try {
            std::filesystem::remove(EMPTY_FILE_PATH);
            std::filesystem::remove(TEST_FILE_PATH);
        } catch (const std::filesystem::filesystem_error &e) {
            FAIL() << "FileReaderTest: error removing temporary file: " << e.what() << '\n';
        }
    }
};

TEST_F(FileReaderTest, AnExceptionIsThrownWhenTheFileDoesNotExist) {
    EXPECT_THROW(auto _ = sodium::IO::read_file("./file_that_does_not_exist.txt"), sodium::IOException);
}

TEST_F(FileReaderTest, FileContentsAreSuccessfullyRead) {
    auto [fileSize, fileContents] = sodium::IO::read_file(TEST_FILE_PATH);

    EXPECT_EQ(TEST_FILE_TEXT, fileContents);
    EXPECT_EQ(fileSize, fileContents.size());
}

TEST_F(FileReaderTest, EmptyFileCanBeRead) {
    auto [fileSize, fileContents] = sodium::IO::read_file(EMPTY_FILE_PATH.string());

    EXPECT_EQ("", fileContents);
    EXPECT_EQ(fileSize, fileContents.size());
}
