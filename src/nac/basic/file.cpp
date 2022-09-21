#include "sodium/nac/basic/file.h"

#include <memory>
#include <string>
#include <utility>

#include "sodium/nac/diagnostics/diagnostic_engine.h"
#include "sodium/nac/diagnostics/fatal_error.h"
#include "sodium/nac/util/file_reader.h"

namespace sodium {

File::File(std::string path) : diagnostic_engine_(std::make_unique<DiagnosticEngine>()), path_(std::move(path)) {
    auto file_contents = util::read_file(path_);

    if (!file_contents.has_value()) {
        auto fatal_error = std::make_unique<FatalError>(FatalErrorKind::FILE_READ_FAIL, path_);
        diagnostic_engine_->diagnose(std::move(fatal_error));
    }

    contents_ = file_contents.value_or("");
}

const std::string &File::path() const {
    return path_;
}

const std::string &File::contents() const {
    return contents_;
}

DiagnosticEngine &File::diagnostics() const {
    return *diagnostic_engine_;
}

} // namespace sodium
