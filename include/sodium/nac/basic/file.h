#ifndef SODIUM_NAC_BASIC_FILE_H
#define SODIUM_NAC_BASIC_FILE_H

#include <string>

namespace sodium {

/// Used to store information about a file, such as its path, size and contents.
class File {
public:
    /// Constructor for File.
    /// The contents of the file are read and stored along with the size of the file on construction.
    /// If there was an error reading the file then the contents will be an empty string.
    /// \param path The path to the file.
    File(std::string path);

    /// \return The path of \c this file.
    [[nodiscard]] const std::string &path() const;

    /// \return The contents of \c this file.
    [[nodiscard]] const std::string &contents() const;

private:
    std::string path_;
    std::string contents_;
};

} // namespace sodium

#endif // SODIUM_NAC_BASIC_FILE_H
