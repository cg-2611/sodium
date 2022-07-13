#ifndef SODIUM_NAC_IO_FILE_READER_H
#define SODIUM_NAC_IO_FILE_READER_H

#include <fstream>
#include <string>

namespace sodium::nac {

class FileReader {
public:
    FileReader(const std::string &filePath);
    ~FileReader();

    size_t readFileContents(std::string &fileContents);
    bool isFileStreamOpen() const;

private:
    std::string path_;
    std::ifstream fileStream_;

    void openFile();
    void closeFile();
};

} // namespace sodium::nac

#endif // SODIUM_NAC_IO_FILE_READER_H
