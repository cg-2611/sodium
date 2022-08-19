#include "sodium/nac/parser/parser.h"

#include <charconv>
#include <memory>

#include "sodium/nac/ast/expr.h"
#include "sodium/nac/lexer/token.h"

namespace sodium {

// Expression = LiteralExpression ;
std::unique_ptr<Expr> Parser::parseExpr() {
    return parseLiteralExpr();
}

// LiteralExpression = NumericLiteralExpression ;
std::unique_ptr<LiteralExpr> Parser::parseLiteralExpr() {
    std::unique_ptr<LiteralExpr> literalExpr(nullptr);

    if (token_->kind() == TokenKind::NUMERIC_LITERAL) {
        literalExpr = parseNumericLiteralExpr();
    }

    // expected numeric literal

    return literalExpr;
}

// NumericLiteralExpression = NumericLiteral ;
std::unique_ptr<NumericLiteralExpr> Parser::parseNumericLiteralExpr() {
    // extract the integer value from the digit characters
    int value;
    std::from_chars(token_->value().data(), token_->position() + token_->length(), value);

    // TODO: check bounds of int size

    return std::make_unique<NumericLiteralExpr>(value);
}

} // namespace sodium
