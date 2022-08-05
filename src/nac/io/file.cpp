#include "sodium/nac/io/file.h"

#include <utility>
#include <string>
#include <tuple>

#include "sodium/nac/io/file_io.h"

namespace sodium {

File::File(std::string path) : path_(std::move(path)), size_(0) {
    std::tie(size_, contents_) = IO::readFile(path_);
}

const std::string &File::path() const noexcept {
    return path_;
}

const std::string &File::contents() const noexcept {
    return contents_;
}

size_t File::size() const noexcept {
    return size_;
}

} // namespace sodium
