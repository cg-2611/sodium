#include "sodium/nac/io/file_reader.h"

#include <cstdio>

#include "gtest/gtest.h"

#include "sodium/nac/exceptions/exception.h"

static const std::string EMPTY_FILE_PATH = "./empty_file.txt";
static const std::string TEST_FILE_PATH = "./test_file.txt";
static const std::string TEST_FILE_TEXT = "Text\n    File used\n\tfor\t\t\nunit testing\n\t  File\nReader\n";

class FileReaderTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        std::FILE *emptyFile = std::fopen(EMPTY_FILE_PATH.c_str(), "w");
        if (!emptyFile) {
            FAIL() << "FileReaderTest: error creating temporary empty file\n";
        }

        std::fclose(emptyFile);

        std::FILE *tempFile = std::fopen(TEST_FILE_PATH.c_str(), "w");
        if (!tempFile) {
            FAIL() << "FileReaderTest: error creating temporary file\n";
        }

        std::fputs(TEST_FILE_TEXT.c_str(), tempFile);
        std::fclose(tempFile);
    }

    static void TearDownTestSuite() {
        if (std::remove(EMPTY_FILE_PATH.c_str()) != 0) {
            FAIL() << "FileReaderTest: error removing empty temporary file\n";
        }

        if (std::remove(TEST_FILE_PATH.c_str()) != 0) {
            FAIL() << "FileReaderTest: error removing temporary file\n";
        }
    }
};

TEST_F(FileReaderTest, AnExceptionIsThrownWhenTheFileDoesNotExist) {
    EXPECT_THROW(
        nac::io::readFile("./file_does_not_exist.txt"),
        nac::Exception
    );
}

TEST_F(FileReaderTest, FileContentsAreSuccessfullyRead) {
    auto [fileSize, fileContents] = nac::io::readFile(TEST_FILE_PATH);

    EXPECT_EQ(TEST_FILE_TEXT, fileContents);
    EXPECT_EQ(fileSize, fileContents.size());
}

TEST_F(FileReaderTest, EmptyFileCanBeRead) {
    auto [fileSize, fileContents] = nac::io::readFile(EMPTY_FILE_PATH);

    EXPECT_EQ("", fileContents);
    EXPECT_EQ(fileSize, fileContents.size());
}
