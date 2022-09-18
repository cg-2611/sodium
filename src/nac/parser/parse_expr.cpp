#include "sodium/nac/parser/parser.h"

#include <memory>
#include <string>

#include "sodium/nac/ast/expr.h"
#include "sodium/nac/basic/source_range.h"
#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/errors/parser_error.h"
#include "sodium/nac/token/token.h"

namespace sodium {

std::unique_ptr<Expr> Parser::parse_expr() {
    switch (token_.kind()) {
        case TokenKind::INTEGER_LITERAL: return parse_integer_literal_expr();
        default: error_expected("expression"); return nullptr;
    }
}

std::unique_ptr<IntegerLiteralExpr> Parser::parse_integer_literal_expr() {
    auto integer_literal_value = token_.value();
    auto integer_literal_range = token_.range();

    if (!expect(TokenKind::INTEGER_LITERAL, "numeric literal")) {
        return nullptr;
    }

    // extract the integer value from the digit characters
    auto integer = std::stoi(integer_literal_value);

    return std::make_unique<IntegerLiteralExpr>(integer, integer_literal_range);
}

} // namespace sodium
