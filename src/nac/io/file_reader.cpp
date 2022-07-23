#include "sodium/nac/io/file_reader.h"

#include <fstream>

#include "sodium/nac/exceptions/exception.h"

std::tuple<size_t, std::string> nac::io::readFile(const std::string& filePath) {
    std::ifstream fileStream(filePath, std::ifstream::ate);

    // fileStream_ is opened on construction
    if (!fileStream.is_open()) {
        throw nac::Exception("error opening file " + filePath);
    }

    // file stream is opened at the end, so tellg() gives the position at end of stream
    // we then cast the std::streampos value returned by tellg() to a size_t, which is
    // permissible as std::streampos is fundamentally an integral type
    // we then return to the beginning of the file stream to read the file
    size_t fileSize = static_cast<size_t>(fileStream.tellg());
    fileStream.seekg(std::ifstream::beg);

    std::string fileContents(fileSize, 0);
    fileStream.read(&fileContents[0], fileSize);

    if (fileStream.fail()) {
        fileStream.close();
        throw nac::Exception("error reading contents of file " + filePath);
    }

    // file stream closed on destruction

    return std::make_tuple(fileSize, fileContents);
}
