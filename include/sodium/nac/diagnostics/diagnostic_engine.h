#ifndef SODIUM_NAC_DIAGNOSTICS_DIAGNOSTIC_ENGINE_H
#define SODIUM_NAC_DIAGNOSTICS_DIAGNOSTIC_ENGINE_H

#include <memory>
#include <ostream>
#include <vector>

#include "sodium/nac/diagnostics/diagnostic.h"

namespace sodium {

/// Used to collect and emit errors encountered during the compilation of a file.
class DiagnosticEngine {
public:
    /// Constructor for DiagnosticEngine.
    DiagnosticEngine() = default;

    /// Pushes a diagnostic to the vector of diagnostics.
    /// \param diagnostic The diagnostic to be added.
    void diagnose(std::unique_ptr<Diagnostic> diagnostic);

    /// Prints the messages of each diagnostic in the vector of diagnostic.
    /// \param os The std::ostream to print the messages to.
    void emit_diagnostics(std::ostream &os) const;

    /// Prints the counts of each kind of diagnostic in the vector of diagnostics.
    /// \param os The std::ostream to print the messages to.
    void emit_diagnostic_stats(std::ostream &os) const;

    /// \return \c true if the vector of diagnostics contains any diagnostics, or \c false otherwise.
    [[nodiscard]] bool has_problems() const;

    /// \return The total number of diagnostics in the vector.
    [[nodiscard]] uint32_t count() const;

    //// \return The total number of diagnostics of kind DiagnosticKind::ERROR.
    [[nodiscard]] uint32_t count_errors() const;

    //// \return The total number of diagnostics of kind DiagnosticKind::FATAL.
    [[nodiscard]] uint32_t count_fatal_errors() const;

    /// \return A raw pointer to the diagnostic at the specified index if the access into the vector was successful, or
    /// \c nullptr otherwise.
    [[nodiscard]] Diagnostic *get(size_t index) const;

private:
    std::vector<std::unique_ptr<Diagnostic>> diagnostics_;
};

} // namespace sodium

#endif // SODIUM_NAC_DIAGNOSTICS_DIAGNOSTIC_ENGINE_H
