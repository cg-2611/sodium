#include "sodium/nac/diagnostics/diagnostic_engine.h"

#include <algorithm>
#include <memory>
#include <ostream>
#include <utility>

#include "sodium/nac/diagnostics/diagnostic.h"

namespace sodium {

void DiagnosticEngine::diagnose(std::unique_ptr<Diagnostic> diagnostic) {
    diagnostics_.push_back(std::move(diagnostic));
}

void DiagnosticEngine::emit_diagnostics(std::ostream &os) const {
    for (auto &&diagnostic : diagnostics_) {
        os << "[nac]: " << diagnostic->message() << '\n';
    }

    emit_diagnostic_stats(os);
}

void DiagnosticEngine::emit_diagnostic_stats(std::ostream &os) const {
    auto errors = count_errors();
    auto fatal_errors = count_fatal_errors();

    if (errors > 0) {
        if (errors == 1) {
            os << "[nac]: generated " << errors << " error\n";
        } else {
            os << "[nac]: generated " << errors << " errors\n";
        }
    } else if (fatal_errors > 0) {
        if (fatal_errors == 1) {
            os << "[nac]: generated " << fatal_errors << " fatal error\n";
        } else {
            os << "[nac]: generated " << fatal_errors << " fatal errors\n";
        }
    }
}

bool DiagnosticEngine::has_problems() const {
    return count() != 0;
}

uint32_t DiagnosticEngine::count() const {
    return diagnostics_.size();
}

uint32_t DiagnosticEngine::count_errors() const {
    return std::count_if(diagnostics_.cbegin(), diagnostics_.cend(),
                         [](const auto &diagnostic) { return diagnostic->diagnostic_kind() == DiagnosticKind::ERROR; });
}

uint32_t DiagnosticEngine::count_fatal_errors() const {
    return std::count_if(diagnostics_.cbegin(), diagnostics_.cend(),
                         [](const auto &diagnostic) { return diagnostic->diagnostic_kind() == DiagnosticKind::FATAL; });
}

Diagnostic *DiagnosticEngine::get(size_t index) const {
    if (index >= count()) {
        return nullptr;
    }

    return diagnostics_[index].get();
}

} // namespace sodium
