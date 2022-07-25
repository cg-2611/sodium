#ifndef NAC_IO_FILE_IO_H
#define NAC_IO_FILE_IO_H

#include <string>
#include <tuple>

namespace nac {
namespace io {

/**
 * Function used to read the contents of a file.
 * @param filePath The path to the file of which the contents are to be read.
 * @return An std::tuple<size_t, std::string> containing the file size in bytes and
 *         the contents of the file as a string.
 * @throws An nac::Exception when the file fails to be opened, or when the file is failed to be read.
 */
[[nodiscard]] std::tuple<size_t, std::string> readFile(const std::string& filePath);

} // namespace io
} // namespace nac

#endif // NAC_IO_FILE_IO_H
