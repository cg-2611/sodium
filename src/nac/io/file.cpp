#include <utility>

#include "sodium/nac/io/file.h"

#include "sodium/nac/io/file_io.h"

namespace nac {

File::File(std::string  path) : path_(std::move(path)) {
    std::tie(size_, contents_) = io::readFile(path_);
}

const std::string& File::path() const {
    return path_;
}

const std::string& File::contents() const {
    return contents_;
}

size_t File::size() const {
    return size_;
}

} // namespace nac
