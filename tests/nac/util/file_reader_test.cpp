#include "sodium/nac/util/file_reader.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <optional>
#include <string>

#include "gtest/gtest.h"

// operator / is overloaded to concatenate file path in std::filesystem
static const std::filesystem::path TEMP_DIRECTORY_PATH = std::filesystem::temp_directory_path() / "sodium_test_files";
static const std::filesystem::path EMPTY_FILE_PATH = TEMP_DIRECTORY_PATH / "empty_file.txt";
static const std::filesystem::path TEST_FILE_PATH = TEMP_DIRECTORY_PATH / "test_file.txt";
static const std::string TEST_FILE_TEXT("Text\n    File used\n\tfor\t\t\nunit testing\n\t  File\nReader\n");

class FileReaderTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        std::filesystem::create_directory(TEMP_DIRECTORY_PATH);

        auto empty_file = std::ofstream(EMPTY_FILE_PATH);

        if (!empty_file.is_open()) {
            FAIL() << "FileReaderTest: error creating temporary file: " << EMPTY_FILE_PATH << '\n';
        }

        empty_file.close();

        auto temp_file = std::ofstream(TEST_FILE_PATH);

        if (!temp_file.is_open()) {
            FAIL() << "FileReaderTest: error creating temporary file: " << TEST_FILE_PATH << '\n';
        }

        temp_file << TEST_FILE_TEXT;

        temp_file.close();
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
    auto file_contents = sodium::util::read_file("./file_that_does_not_exist.txt");

    EXPECT_FALSE(file_contents.has_value());
}

TEST_F(FileReaderTest, FileContentsAreSuccessfullyRead) {
    auto file_contents = sodium::util::read_file(TEST_FILE_PATH);

    ASSERT_TRUE(file_contents.has_value());
    EXPECT_EQ(TEST_FILE_TEXT, file_contents.value());
}

TEST_F(FileReaderTest, EmptyFileCanBeRead) {
    auto file_contents = sodium::util::read_file(EMPTY_FILE_PATH.string());

    ASSERT_TRUE(file_contents.has_value());
    EXPECT_EQ("", file_contents.value());
}
