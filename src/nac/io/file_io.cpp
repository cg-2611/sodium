#include "sodium/nac/io/file_io.h"

#include <cstdio>
#include <filesystem>

#include "sodium/nac/exceptions/io_exception.h"

namespace nac {

[[nodiscard]] std::tuple<size_t, std::string> io::readFile(const std::string& filePath) {
    // create a unique pointer for the file descriptor
    // the file is opened in read mode on construction and uses fclose in the custom deleter
    // this ensures that the file is always closed on destruction of the unique pointer
    std::unique_ptr<std::FILE, decltype(&std::fclose)> file(std::fopen(filePath.c_str(), "r"), &std::fclose);

    if (!file) {
        throw IOException(Error::FILE_OPEN_FAIL, filePath);
    }

    size_t fileSize = static_cast<size_t>(std::filesystem::file_size(filePath));

    std::string fileContents(fileSize, '\0');
    size_t readLength = std::fread(fileContents.data(), sizeof(char), fileSize, file.get());

    // if these two values differ, then the file has not been read correctly
    if (readLength != fileSize) {
        throw IOException(Error::FILE_READ_FAIL, filePath);
    }

    return std::make_tuple(fileSize, fileContents);
}

} // namespace nac
