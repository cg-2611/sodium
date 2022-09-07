#include "sodium/nac/parser/parser.h"

#include <memory>
#include <string_view>
#include <utility>

#include <gtest/gtest.h>

#include "sodium/nac/ast/expr.h"
#include "sodium/nac/token/token.h"

/*
    tests expression:
        2
*/
TEST(ParseExprTest, ParserCorrectlyDispatchesToParseANumericLiteralExpression) {
    std::string_view expression_string("2");

    sodium::Parser parser(expression_string);
    auto expr(parser.parse_expr());

    EXPECT_EQ(sodium::ASTNodeKind::EXPR, expr->node_kind());
    EXPECT_EQ(sodium::ExprKind::NUMERIC_LITERAL, expr->expr_kind());
}

/*
    tests numeric literal expression:
        2
*/
TEST(ParseExprTest, ParserCorrectlyParsesASingleDigitNumericLiteral) {
    sodium::Parser parser("2");
    auto numeric_literal_expr(parser.parse_numeric_literal_expr());

    EXPECT_EQ(sodium::ASTNodeKind::EXPR, numeric_literal_expr->node_kind());
    EXPECT_EQ(sodium::ExprKind::NUMERIC_LITERAL, numeric_literal_expr->expr_kind());

    EXPECT_EQ(2, numeric_literal_expr->value());
}

/*
    tests numeric literal expression:
        567
*/
TEST(ParseExprTest, ParserCorrectlyParsesAMultiDigitNumericLiteral) {
    sodium::Parser parser("567");
    auto numeric_literal_expr(parser.parse_numeric_literal_expr());

    EXPECT_EQ(sodium::ASTNodeKind::EXPR, numeric_literal_expr->node_kind());
    EXPECT_EQ(sodium::ExprKind::NUMERIC_LITERAL, numeric_literal_expr->expr_kind());

    EXPECT_EQ(567, numeric_literal_expr->value());
}
