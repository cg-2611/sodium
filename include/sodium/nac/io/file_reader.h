#ifndef NAC_IO_FILE_READER_H
#define NAC_IO_FILE_READER_H

#include <string>

namespace nac {
namespace io {

/**
 * Function used to read the contents of a file.
 * @param filePath The path to the file of which the contents are to be read.
 * @param fileContents [out] The string where the contents of the file are read into.
 * @return The size in bytes of the file read.
 * @throws An nac::Exception when the file fails to be opened, or when the file is failed to be read.
*/
size_t readFile(const std::string &filePath, std::string &fileContents);

} // namespace nac::io
} // namespace nac

#endif // NAC_IO_FILE_READER_H
