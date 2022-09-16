#include "sodium/nac/basic/file.h"

#include <string>
#include <utility>

#include "sodium/nac/util/file_reader.h"

namespace sodium {

File::File(std::string path) : path_(std::move(path)), contents_(util::read_file(path_).value_or("")) {}

const std::string &File::path() const {
    return path_;
}

const std::string &File::contents() const {
    return contents_;
}

} // namespace sodium
