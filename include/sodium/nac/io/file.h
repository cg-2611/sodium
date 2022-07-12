#ifndef SODIUM_NAC_FILE_H
#define SODIUM_NAC_FILE_H

#include <string>

namespace sodium::nac {

class File {
private:
    std::string path_;
    size_t size_;
    std::string contents_;

public:
    File(const char *filePath);
    ~File() {};

    const std::string getPath() const;
    size_t getSize() const;
    const std::string getContents() const;
};

} // namespace sodium::nac

#endif // SODIUM_NAC_FILE_H
