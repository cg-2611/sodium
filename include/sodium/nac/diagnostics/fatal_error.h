#ifndef SODIUM_NAC_DIAGNOSTICS_FATAL_ERROR_H
#define SODIUM_NAC_DIAGNOSTICS_FATAL_ERROR_H

#include <string>
#include <string_view>

#include "sodium/nac/diagnostics/diagnostic.h"

namespace sodium {

/// An enum of all the fatal errors that can be generated in the Sodium compiler.
enum class FatalErrorKind {
    FILE_READ_FAIL
};

/// Used to represent a fatal error in the Sodium compiler.
class FatalError : public Diagnostic {
public:
    /// Constructor for FatalError.
    /// Forms the error message that describes the fatal error.
    /// \param kind The kind of the fatal error.
    /// \param path The path of the file that the fatal error has occurred in.
    FatalError(FatalErrorKind kind, const std::string &path);

    /// \return The kind of \c this fatal error.
    [[nodiscard]] FatalErrorKind kind() const;

    /// \return The message that describes \c this fatal error.
    [[nodiscard]] const std::string &message() const override;

    /// Generate a description of a FatalErrorKind.
    /// \param kind The kind of fatal error to generate the description for.
    /// \return The description of the given fatal error.
    static constexpr std::string_view description_from_kind(FatalErrorKind kind);

private:
    FatalErrorKind kind_;
    std::string message_;
};

} // namespace sodium

#endif // SODIUM_NAC_DIAGNOSTICS_FATAL_ERROR_H
