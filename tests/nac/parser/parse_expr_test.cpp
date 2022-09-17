#include "sodium/nac/parser/parser.h"

#include <string_view>

#include "gtest/gtest.h"

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/expr.h"
#include "sodium/nac/lexer/lexer.h"

/*
    tests expression:
        2
*/
TEST(ParseExprTest, ParserCorrectlyDispatchesToParseAIntegerLiteralExpression) {
    auto src = std::string_view("2");
    auto token_buffer = sodium::Lexer(src).tokenize();

    auto parser = sodium::Parser(token_buffer);
    auto expr = parser.parse_expr();

    EXPECT_EQ(sodium::ASTNodeKind::EXPR, expr->node_kind());
    EXPECT_EQ(sodium::ExprKind::INTEGER_LITERAL, expr->expr_kind());
}

/*
    tests integer literal expression:
        2
*/
TEST(ParseExprTest, ParserCorrectlyParsesASingleDigitIntegerLiteral) {
    auto src = std::string_view("2");
    auto token_buffer = sodium::Lexer(src).tokenize();

    auto parser = sodium::Parser(token_buffer);
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
    auto src = std::string_view("567");
    auto token_buffer = sodium::Lexer(src).tokenize();

    auto parser = sodium::Parser(token_buffer);
    auto integer_literal_expr = parser.parse_integer_literal_expr();

    EXPECT_EQ(sodium::ASTNodeKind::EXPR, integer_literal_expr->node_kind());
    EXPECT_EQ(sodium::ExprKind::INTEGER_LITERAL, integer_literal_expr->expr_kind());

    EXPECT_EQ(567, integer_literal_expr->value());
}
