#ifndef SODIUM_NAC_PARSER_PARSER_DIAGNOSTICS_H
#define SODIUM_NAC_PARSER_PARSER_DIAGNOSTICS_H

#include <optional>
#include <string_view>

#include "sodium/nac/diagnostics/error.h"

namespace sodium {

class Token;

/// An enum of the kinds of parser errors that can be generated by the Sodium compiler parser.
enum class ParserErrorKind {
    // reserved tokens
    EXPECTED_KEYWORD_FUNC,
    EXPECTED_KEYWORD_RETURN,
    EXPECTED_TYPE,

    // literal tokens
    EXPECTED_IDENTIFIER,
    EXPECTED_INTEGER_LITERAL,

    // symbol tokens
    EXPECTED_ARROW,
    EXPECTED_LEFT_BRACE_BLOCK,
    EXPECTED_LEFT_PAREN_PARAMETER_LIST,
    EXPECTED_RIGHT_BRACE_BLOCK,
    EXPECTED_RIGHT_PAREN_PARAMETER_LIST,
    EXPECTED_SEMICOLON,

    // ast constructs
    EXPECTED_DECLARATION,
    EXPECTED_EXPRESSION,
    EXPECTED_STATEMENT
};

/// Derived class of Error, used to represent an error that is diagnosed by the parser.
class ParserError : public Error {
public:
    /// Constructor for ParserError.
    /// Forms the error message that describes the parser error.
    /// \param kind The kind of the parser error.
    /// \param token The token that has caused the error.
    ParserError(ParserErrorKind kind, const Token &token);

    /// \return The kind of \c this parser error.
    [[nodiscard]] ParserErrorKind kind() const;

    /// \return The message that describes \c this parser error.
    [[nodiscard]] const std::string &message() const override;

    /// Generate a description of a ParserErrorKind.
    /// \param kind The kind of parser error to generate the description for.
    /// \return The description of the given parser error.
    static constexpr std::string_view description_from_kind(ParserErrorKind kind);

private:
    ParserErrorKind kind_;
    std::string message_;
};

} // namespace sodium

#endif // SODIUM_NAC_PARSER_PARSER_DIAGNOSTICS_H
