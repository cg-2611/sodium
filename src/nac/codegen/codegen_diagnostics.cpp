#include "sodium/nac/codegen/codegen_diagnostics.h"

#include <string>
#include <string_view>

#include "sodium/nac/basic/source_location.h"

namespace sodium {

CodegenError::CodegenError(CodegenErrorKind kind, SourceLocation location)
        : Error(ErrorKind::CODEGEN, location),
          kind_(kind) {
    message_ += Error::message() + " ";
    message_ += description_from_kind(kind_);
}

CodegenErrorKind CodegenError::kind() const {
    return kind_;
}

const std::string &CodegenError::message() const {
    return message_;
}

constexpr std::string_view CodegenError::description_from_kind(CodegenErrorKind kind) {
    switch (kind) {
        case CodegenErrorKind::NO_RETURN_EXPRESSION: return "no return expression";
        case CodegenErrorKind::NO_RETURN_STATEMENT: return "no return statement";
        default: "unkown codegen error";
    }
}

} // namespace sodium
