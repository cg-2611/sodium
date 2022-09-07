#include "sodium/nac/io/file_io.h"

#include <cstdio>
#include <filesystem>
#include <memory>
#include <string>
#include <tuple>

#include "sodium/nac/exceptions/exception.h"
#include "sodium/nac/exceptions/io_exception.h"

namespace sodium {

std::tuple<size_t, std::string> IO::read_file(const std::string &file_path) {
    // create a unique pointer for the file descriptor
    // the file is opened in read mode on construction and uses fclose in the custom deleter
    // this ensures that the file is always closed on destruction of the unique pointer
    std::unique_ptr<std::FILE, decltype(&std::fclose)> file(std::fopen(file_path.c_str(), "r"), &std::fclose);

    if (!file) {
        throw IOException(ExceptionKind::FILE_OPEN_FAIL, file_path);
    }

    size_t file_size = static_cast<size_t>(std::filesystem::file_size(file_path));

    std::string file_contents(file_size, '\0');
    size_t read_length = std::fread(file_contents.data(), sizeof(char), file_size, file.get());

    // if these two values differ, then the file has not been read correctly
    if (read_length != file_size) {
        throw IOException(ExceptionKind::FILE_READ_FAIL, file_path);
    }

    return std::make_tuple(file_size, file_contents);
}

} // namespace sodium
