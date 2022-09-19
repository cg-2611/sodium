#ifndef SODIUM_NAC_DIAGNOSTICS_ERROR_H
#define SODIUM_NAC_DIAGNOSTICS_ERROR_H

#include <string>

#include "sodium/nac/basic/source_location.h"
#include "sodium/nac/diagnostics/diagnostic.h"

namespace sodium {

enum class ErrorKind {
    LEXER,
    PARSER
};

/// A base class for errors generated in the Sodium compiler.
class Error : public Diagnostic {
public:
    Error(const Error &) = delete;
    Error &operator=(const Error &) = delete;

    Error(Error &&) noexcept = delete;
    Error &operator=(Error &&) noexcept = delete;

    ~Error() override = default;

    /// \return The kind of \c this error.
    [[nodiscard]] ErrorKind error_kind() const;

    /// \return The location of \c this error.
    [[nodiscard]] SourceLocation location() const;

    /// \return The message that describes \c this error.
    [[nodiscard]] const std::string &message() const override;

protected:
    /// Constructor for Error.
    /// Forms the message that describes the location of the error,
    /// \param kind The kind of the error.
    /// \param location The location of the error.
    Error(ErrorKind kind, SourceLocation location);

private:
    ErrorKind kind_;
    SourceLocation location_;
    std::string message_;
};

} // namespace sodium

#endif // SODIUM_NAC_DIAGNOSTICS_ERROR_H
