#ifndef SODIUM_NAC_FILE_READER_H
#define SODIUM_NAC_FILE_READER_H

#include <string>

namespace sodium::nac {

namespace FileIO {

    std::string readFileContents(std::string filePath, size_t *fileSize);

} // namespace sodium::nac::FileIO

} // namespace sodium::nac

#endif // SODIUM_NAC_FILE_READER_H
