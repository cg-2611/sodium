#include "sodium/nac/io/file_io.h"

#include <cstdio>
#include <filesystem>

#include <gtest/gtest.h>

#include "sodium/nac/exceptions/exception.h"

// operator / is overloaded to concatenate file path in std::filesystem
static const std::filesystem::path TEMP_DIRECTORY_PATH = std::filesystem::temp_directory_path() / "sodium_test_files";
static const std::filesystem::path EMPTY_FILE_PATH = TEMP_DIRECTORY_PATH / "empty_file.txt";
static const std::filesystem::path TEST_FILE_PATH = TEMP_DIRECTORY_PATH / "test_file.txt";
static const std::string TEST_FILE_TEXT("Text\n    File used\n\tfor\t\t\nunit testing\n\t  File\nReader\n");

class FileReaderTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        std::filesystem::create_directory(TEMP_DIRECTORY_PATH);

        std::unique_ptr<std::FILE, decltype(&std::fclose)> emptyFile(std::fopen(EMPTY_FILE_PATH.c_str(), "w"),
                                                                     &std::fclose);
        if (!emptyFile) {
            FAIL() << "FileReaderTest: error creating temporary file: " << EMPTY_FILE_PATH << '\n';
        }

        std::unique_ptr<std::FILE, decltype(&std::fclose)> tempFile(std::fopen(TEST_FILE_PATH.c_str(), "w"),
                                                                    &std::fclose);
        if (!tempFile) {
            FAIL() << "FileReaderTest: error creating temporary file: " << TEST_FILE_PATH << '\n';
        }

        std::fputs(TEST_FILE_TEXT.c_str(), tempFile.get());
    }

    static void TearDownTestSuite() {
        try {
            std::filesystem::remove(EMPTY_FILE_PATH);
            std::filesystem::remove(TEST_FILE_PATH);
        } catch (const std::filesystem::filesystem_error& e) {
            FAIL() << "FileReaderTest: error removing temporary file: " << e.what() << '\n';
        }
    }
};

TEST_F(FileReaderTest, AnExceptionIsThrownWhenTheFileDoesNotExist) {
    EXPECT_THROW(auto _ = nac::io::readFile("./file_that_does_not_exist.txt"), nac::Exception);
}

TEST_F(FileReaderTest, FileContentsAreSuccessfullyRead) {
    auto [fileSize, fileContents] = nac::io::readFile(TEST_FILE_PATH);

    EXPECT_EQ(TEST_FILE_TEXT, fileContents);
    EXPECT_EQ(fileSize, fileContents.size());
}

TEST_F(FileReaderTest, EmptyFileCanBeRead) {
    auto [fileSize, fileContents] = nac::io::readFile(EMPTY_FILE_PATH.string());

    EXPECT_EQ("", fileContents);
    EXPECT_EQ(fileSize, fileContents.size());
}
