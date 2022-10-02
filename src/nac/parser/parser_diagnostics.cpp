#include "sodium/nac/parser/parser_diagnostics.h"

#include <optional>
#include <string>
#include <string_view>

#include "sodium/nac/token/token.h"

namespace sodium {

ParserError::ParserError(ParserErrorKind kind, const Token &token)
        : Error(ErrorKind::PARSER, token.range().start()),
          kind_(kind) {
    message_ += Error::message() + " ";
    message_ += description_from_kind(kind_);
    message_ += ", found \'" + token.value() + "\'";
}

ParserErrorKind ParserError::kind() const {
    return kind_;
}

const std::string &ParserError::message() const {
    return message_;
}

constexpr std::string_view ParserError::description_from_kind(ParserErrorKind kind) {
    switch (kind) {
        // reserved tokens
        case ParserErrorKind::EXPECTED_KEYWORD_FUNC: return "expected keyword \'func\'";
        case ParserErrorKind::EXPECTED_KEYWORD_RETURN: return "expected keyword \'return\'";
        case ParserErrorKind::EXPECTED_TYPE: return "expected type";

        // literal tokens
        case ParserErrorKind::EXPECTED_IDENTIFIER: return "expected identifier";
        case ParserErrorKind::EXPECTED_INTEGER_LITERAL: return "expected integer literal";

        // symbol tokens
        case ParserErrorKind::EXPECTED_ARROW: return "expected \'->\'";
        case ParserErrorKind::EXPECTED_LEFT_BRACE_BLOCK: return "expected \'{\' to begin block";
        case ParserErrorKind::EXPECTED_LEFT_PAREN_PARAMETER_LIST: return "expected \'(\' to begin parameter list";
        case ParserErrorKind::EXPECTED_RIGHT_BRACE_BLOCK: return "expected \'}\' to end block";
        case ParserErrorKind::EXPECTED_RIGHT_PAREN_PARAMETER_LIST: return "expected \')\' to ed parameter list";
        case ParserErrorKind::EXPECTED_SEMICOLON: return "expected \';\'";

        // ast constructs
        case ParserErrorKind::EXPECTED_DECLARATION: return "expected declaration";
        case ParserErrorKind::EXPECTED_EXPRESSION: return "expected expression";
        case ParserErrorKind::EXPECTED_STATEMENT: return "expected statement";
        default: return "unkown parser error";
    }
}

} // namespace sodium
