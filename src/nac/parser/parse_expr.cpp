#include "sodium/nac/parser/parser.h"

#include <charconv>
#include <memory>

#include "sodium/nac/ast/expr.h"
#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/errors/parser_error.h"
#include "sodium/nac/lexer/token.h"

namespace sodium {

std::unique_ptr<Expr> Parser::parseExpr() {
    return parseLiteralExpr();
}

std::unique_ptr<LiteralExpr> Parser::parseLiteralExpr() {
    switch (token_.kind()) {
        case TokenKind::NUMERIC_LITERAL: return parseNumericLiteralExpr();
        default: return nullptr;
    }
}

std::unique_ptr<NumericLiteralExpr> Parser::parseNumericLiteralExpr() {
    int value{};

    if (!match(TokenKind::NUMERIC_LITERAL)) {
        return nullptr;
    }

    // extract the integer value from the digit characters
    std::from_chars(token_.value().data(), token_.location().position() + token_.length(), value);

    // TODO: check bounds of int size

    return std::make_unique<NumericLiteralExpr>(value);
}

} // namespace sodium
