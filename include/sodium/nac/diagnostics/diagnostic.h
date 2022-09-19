#ifndef SODIUM_NAC_DIAGNOSTICS_DIAGNOSTIC_H
#define SODIUM_NAC_DIAGNOSTICS_DIAGNOSTIC_H

#include <string>

namespace sodium {

/// An enum of the kinds of diagnostics generated by the Sodium compiler.
enum class DiagnosticKind {
    ERROR,
    FATAL
};

/// Base class for diagnostics generated by the Sodium compiler.
class Diagnostic {
public:
    Diagnostic(const Diagnostic &) = delete;
    Diagnostic &operator=(const Diagnostic &) = delete;

    Diagnostic(Diagnostic &&) noexcept = delete;
    Diagnostic &operator=(Diagnostic &&) noexcept = delete;

    virtual ~Diagnostic() = default;

    /// \return The kind of \c this diagnostic.
    [[nodiscard]] DiagnosticKind diagnostic_kind() const;

    /// \return The message describing the problem of this diagnostic.
    [[nodiscard]] virtual const std::string &message() const = 0;

protected:
    /// Constructor for Diagnostic.
    /// \param kind The kind of the diagnostic.
    Diagnostic(DiagnosticKind kind);

private:
    DiagnosticKind kind_;
};

} // namespace sodium

#endif // SODIUM_NAC_DIAGNOSTICS_DIAGNOSTIC_H