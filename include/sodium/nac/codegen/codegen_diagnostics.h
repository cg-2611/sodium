#ifndef SODIUM_NAC_CODEGEN_CODEGEN_DIAGNOSTIC_H
#define SODIUM_NAC_CODEGEN_CODEGEN_DIAGNOSTIC_H

#include "sodium/nac/diagnostics/error.h"

namespace sodium {

class SourceLocation;

enum class CodegenErrorKind {
    NO_RETURN_STATEMENT,
    NO_RETURN_EXPRESSION
};

/// Derived class of Error, used to represent an error that is diagnosed during codegen.
class CodegenError : public Error {
public:
    /// Constructor for CodegenError.
    /// Forms the error message that describes the codegen error.
    /// \param kind The kind of the codegen error.
    /// \param location The SourceLocation of the error.
    CodegenError(CodegenErrorKind kind, SourceLocation location);

    /// \return The kind of \c this codegen error.
    [[nodiscard]] CodegenErrorKind kind() const;

    /// \return The message that describes \c this codegen error.
    [[nodiscard]] const std::string &message() const override;

    /// Generate a description of a CodegenErrorKind.
    /// \param kind The kind of codegen error to generate the description for.
    /// \return The description of the given codegen error.
    static constexpr std::string_view description_from_kind(CodegenErrorKind kind);

private:
    CodegenErrorKind kind_;
    std::string message_;
};

} // namespace sodium

#endif // SODIUM_NAC_CODEGEN_CODEGEN_DIAGNOSTIC_H
