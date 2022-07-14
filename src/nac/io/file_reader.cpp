#include "sodium/nac/io/file_reader.h"

#include <iostream>

#include "sodium/nac/error/error.h"

sodium::nac::FileReader::FileReader(const std::string &filePath) {
    path_ = filePath;
    openFile();

    if (!isFileStreamOpen()) {
        throw sodium::nac::NACException("error opening file " + path_);
    }
}

sodium::nac::FileReader::~FileReader() {
    closeFile();
}

size_t sodium::nac::FileReader::readFileContents(std::string &fileContents) {
    // file stream is opened at the end, so tellg() gives the position at end of stream
    // we then cast the std::streampos value returned by tellg() to a size_t, which is
    // permissible as std::streampos is fundamentally an integral type
    // we then return to the beginning of the file stream to read the file
    size_t fileSize = static_cast<size_t>(fileStream_.tellg());
    fileStream_.seekg(std::ifstream::beg);

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
}

void sodium::nac::FileReader::closeFile() {
    fileStream_.close();
}
