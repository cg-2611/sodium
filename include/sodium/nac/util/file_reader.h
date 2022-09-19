#ifndef SODIUM_NAC_UTIL_FILE_READER_H
#define SODIUM_NAC_UTIL_FILE_READER_H

#include <optional>
#include <string>

namespace sodium::util {

/// Reads the contents of a file.
/// \param file_path The path to the file of which the contents are to be read.
/// \return An std::optional<std::string> that has the value of the contents of the file as a string if reading the file
/// was successful, or \c nullopt otherwise.
std::optional<std::string> read_file(const std::string &file_path);

} // namespace sodium::util

#endif // SODIUM_NAC_UTIL_FILE_READER_H
