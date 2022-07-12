#include "sodium/nac/io/file.h"

#include "sodium/nac/error/error.h"
#include "sodium/nac/io/file_io.h"

sodium::nac::File::File(const char *filePath) {
    path_ = std::string(filePath);
    contents_ = sodium::nac::FileIO::readFileContents(path_, &size_);
}

const std::string sodium::nac::File::getPath() const {
    return path_;
}

size_t sodium::nac::File::getSize() const {
    return size_;
}

const std::string sodium::nac::File::getContents() const {
    return contents_;
}
