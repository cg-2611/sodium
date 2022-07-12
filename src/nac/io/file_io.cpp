#include "sodium/nac/io/file_io.h"

#include <cstdlib>
#include <fstream>

#include "sodium/nac/error/error.h"

std::string sodium::nac::FileIO::readFileContents(std::string filePath, size_t *fileSize) {
    // ifstream is opened on construction
    std::ifstream inputFileStream = std::ifstream(filePath, std::ifstream::binary | std::ifstream::ate);

    if (!inputFileStream.is_open()) {
        throw sodium::nac::NACException(EXIT_FAILURE, "error opening file: " + filePath);
    }

    // file is opened at end of stream, std::ifstream.tellg() gives current position in input stream
    // the std::streampos is casted to size_t, this can be done as std::streampos is an integral type
    *fileSize = (size_t)inputFileStream.tellg();
    inputFileStream.seekg(std::ios::beg);

    // create a string that is large enough to store file contents and read the file contents into the string
    std::string fileContents = std::string(*fileSize, 0);
    inputFileStream.read(&fileContents[0], *fileSize);

    inputFileStream.close();

    if (inputFileStream.fail()) {
        throw sodium::nac::NACException(EXIT_FAILURE, "error reading contents of file: " + filePath);
    }

    return fileContents;
}
