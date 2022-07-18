#include "sodium/nac/io/file.h"

#include "sodium/nac/io/file_reader.h"

nac::File::File(const std::string &path) : path_(path) {
    size_ = nac::io::readFile(path_, contents_);
}

const std::string & nac::File::getPath() const {
    return path_;
}

const std::string & nac::File::getContents() const {
    return contents_;
}

size_t nac::File::getSize() const {
    return size_;
}
