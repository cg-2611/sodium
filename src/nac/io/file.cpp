#include "sodium/nac/io/file.h"

#include "sodium/nac/io/file_reader.h"

nac::File::File(const std::string &path) : path_(path) {
    std::tie(size_, contents_) = nac::io::readFile(path_);
}

const std::string & nac::File::path() const {
    return path_;
}

const std::string & nac::File::contents() const {
    return contents_;
}

size_t nac::File::size() const {
    return size_;
}
