#include "sodium/nac/io/file.h"

#include "sodium/nac/error/error.h"
#include "sodium/nac/io/file_reader.h"

sodium::nac::File::File(const std::string &filePath) {
    sodium::nac::FileReader fileReader = sodium::nac::FileReader(filePath);

    path_ = filePath;
    size_ = fileReader.readFileContents(contents_);
}

const std::string sodium::nac::File::getPath() const {
    return path_;
}

const std::string sodium::nac::File::getContents() const {
    return contents_;
}

size_t sodium::nac::File::getSize() const {
    return size_;
}
