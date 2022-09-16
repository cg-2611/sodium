#include "sodium/nac/parser/parser.h"

#include <string_view>

#include "gtest/gtest.h"

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/expr.h"

/*
    tests expression:
        2
*/
TEST(ParseExprTest, ParserCorrectlyDispatchesToParseAIntegerLiteralExpression) {
    auto parser = sodium::Parser("2");
    auto expr = parser.parse_expr();

    EXPECT_EQ(sodium::ASTNodeKind::EXPR, expr->node_kind());
    EXPECT_EQ(sodium::ExprKind::INTEGER_LITERAL, expr->expr_kind());
}

/*
    tests integer literal expression:
        2
*/
TEST(ParseExprTest, ParserCorrectlyParsesASingleDigitIntegerLiteral) {
    auto parser = sodium::Parser("2");
    auto integer_literal_expr = parser.parse_integer_literal_expr();

    EXPECT_EQ(sodium::ASTNodeKind::EXPR, integer_literal_expr->node_kind());
    EXPECT_EQ(sodium::ExprKind::INTEGER_LITERAL, integer_literal_expr->expr_kind());

    EXPECT_EQ(2, integer_literal_expr->value());
}

/*
    tests integer literal expression:
        567
*/
TEST(ParseExprTest, ParserCorrectlyParsesAMultiDigitIntegerLiteral) {
    auto parser = sodium::Parser("567");
    auto integer_literal_expr = parser.parse_integer_literal_expr();

    EXPECT_EQ(sodium::ASTNodeKind::EXPR, integer_literal_expr->node_kind());
    EXPECT_EQ(sodium::ExprKind::INTEGER_LITERAL, integer_literal_expr->expr_kind());

    EXPECT_EQ(567, integer_literal_expr->value());
}
