#include "sodium/nac/parser/parser.h"

#include <charconv>
#include <memory>
#include <unordered_set>

#include "sodium/nac/ast/expr.h"
#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/errors/parser_error.h"
#include "sodium/nac/lexer/token.h"

namespace sodium {

static const std::unordered_set<TokenKind> EXPR_SYNCHRONIZING_TOKENS = {TokenKind::RIGHT_BRACE, TokenKind::SEMICOLON};

std::unique_ptr<Expr> Parser::parseExpr() {
    // std::unique_ptr<LiteralExpr> literalExpr(parseLiteralExpr());
    // if (literalExpr == nullptr) {
    //     // expected expression
    //     ErrorManager::addError<ParserError>(ErrorKind::SYNTAX_ERROR, token_, "expected expression");
    //     synchronize(EXPR_SYNCHRONIZING_TOKENS);
    // }
    // return literalExpr;
    return parseLiteralExpr();
}

std::unique_ptr<LiteralExpr> Parser::parseLiteralExpr() {
    switch (token_->kind()) {
        case TokenKind::NUMERIC_LITERAL: return parseNumericLiteralExpr();
        default:
            // expected numeric literal
            ErrorManager::addError<ParserError>(ErrorKind::SYNTAX_ERROR, token_, "expected numeric literal");
            return nullptr;
    }
}

std::unique_ptr<NumericLiteralExpr> Parser::parseNumericLiteralExpr() {
    // extract the integer value from the digit characters
    int value;
    std::from_chars(token_->value().data(), token_->position() + token_->length(), value);

    // TODO: check bounds of int size

    return std::make_unique<NumericLiteralExpr>(value);
}

} // namespace sodium
