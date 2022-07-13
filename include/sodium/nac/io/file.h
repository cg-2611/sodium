#ifndef SODIUM_NAC_IO_FILE_H
#define SODIUM_NAC_IO_FILE_H

#include <string>

namespace sodium::nac {

class File {
public:
    File(const std::string &filePath);
    ~File() {}

    const std::string getPath() const;
    const std::string getContents() const;
    size_t getSize() const;

private:
    std::string path_;
    std::string contents_;
    size_t size_;
};

} // namespace sodium::nac

#endif // SODIUM_NAC_IO_FILE_H
