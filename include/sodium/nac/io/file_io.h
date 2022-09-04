#ifndef SODIUM_NAC_IO_FILE_IO_H
#define SODIUM_NAC_IO_FILE_IO_H

#include <string>
#include <tuple>

namespace sodium::IO {

/**
 * @brief Function used to read the contents of a file.
 *
 * @param filePath The path as an std::string to the file of which the contents are to be read.
 * @return std::tuple<size_t, std::string> that contains the file size in bytes and
 *         the contents of the file as an std::string.
 * @throws An IOException when the file fails to be opened, or when the file is failed to be read.
 */
std::tuple<size_t, std::string> readFile(const std::string &filePath);

} // namespace sodium::IO

#endif // SODIUM_NAC_IO_FILE_IO_H
