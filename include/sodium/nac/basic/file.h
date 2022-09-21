#ifndef SODIUM_NAC_BASIC_FILE_H
#define SODIUM_NAC_BASIC_FILE_H

#include <memory>
#include <string>

namespace sodium {

class DiagnosticEngine;

/// Used to store information about a file.
class File {
public:
    /// Constructor for File.
    /// The contents of the file are read and stored along with the size of the file on construction.
    /// If there was an error reading the file then the contents will be an empty string and the error is diagnosed in
    /// the diagnostic engine of the file.
    /// \param path The path to the file.
    File(std::string path);

    /// \return The diagnostic engine for \c this file.
    [[nodiscard]] DiagnosticEngine &diagnostics() const;

    /// \return The path of \c this file.
    [[nodiscard]] const std::string &path() const;

    /// \return The contents of \c this file.
    [[nodiscard]] const std::string &contents() const;

private:
    std::unique_ptr<DiagnosticEngine> diagnostic_engine_;
    std::string path_;
    std::string contents_;
};

} // namespace sodium

#endif // SODIUM_NAC_BASIC_FILE_H
