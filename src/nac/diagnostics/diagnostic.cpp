#include "sodium/nac/diagnostics/diagnostic.h"

namespace sodium {

Diagnostic::Diagnostic(DiagnosticKind kind) : kind_(kind) {}

DiagnosticKind Diagnostic::diagnostic_kind() const {
    return kind_;
}

} // namespace sodium
