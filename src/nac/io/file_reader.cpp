#include "sodium/nac/io/file_reader.h"

#include <iostream>

#include "sodium/nac/error/error.h"

sodium::nac::FileReader::FileReader(const std::string &filePath) {
    path_ = filePath;
    openFile();
}

sodium::nac::FileReader::~FileReader() {
    closeFile();
}

size_t sodium::nac::FileReader::readFileContents(std::string &fileContents) {
    // the file is opened at end of the stream, std::ifstream::tellg() gives
    // the current position in the input stream the std::streampos is casted to
    // size_t, this can be done as the std::streampos returned by
    // std::ifstream::tellg() is an integral type
    size_t fileSize = (size_t)fileStream_.tellg();
    fileStream_.seekg(std::ios::beg);

    fileContents = std::string(fileSize, 0);
    fileStream_.read(&fileContents[0], fileSize);

    if (fileStream_.fail()) {
        closeFile();
        throw sodium::nac::NACException("error reading contents of file " + path_);
    }

    return fileSize;
}

bool sodium::nac::FileReader::isFileStreamOpen() const {
    return fileStream_.is_open();
}

void sodium::nac::FileReader::openFile() {
    // ifstream is opened on construction
    fileStream_ = std::ifstream(path_, std::ifstream::binary | std::ifstream::ate);

    if (!fileStream_.is_open()) {
        throw sodium::nac::NACException("error opening file " + path_);
    }
}

void sodium::nac::FileReader::closeFile() {
    fileStream_.close();
}
