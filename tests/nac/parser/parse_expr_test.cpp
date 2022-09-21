#include "sodium/nac/parser/parser.h"

#include <string_view>

#include "gtest/gtest.h"

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/expr.h"
#include "sodium/nac/diagnostics/diagnostic_engine.h"
#include "sodium/nac/lexer/lexer.h"
#include "sodium/nac/parser/parser_diagnostics.h"

/*
    tests expression:
        2
*/
TEST(ParseExprTest, ParserCorrectlyDispatchesToParseAIntegerLiteralExpression) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("2");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto expr = parser.parse_expr();

    EXPECT_EQ(sodium::ASTNodeKind::EXPR, expr->node_kind());
    EXPECT_EQ(sodium::ExprKind::INTEGER_LITERAL, expr->expr_kind());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

/*
    tests expression:
        func
*/
TEST(ParseExprTest, ParserDiagnosesParserErrorForInvalidExpression) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("func");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto expr = parser.parse_expr();

    EXPECT_EQ(nullptr, expr);

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_EXPRESSION, parser_error->kind());
}

/*
    tests integer literal expression:
        2
*/
TEST(ParseExprTest, ParserCorrectlyParsesASingleDigitIntegerLiteral) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("2");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto integer_literal_expr = parser.parse_integer_literal_expr();

    EXPECT_EQ(sodium::ASTNodeKind::EXPR, integer_literal_expr->node_kind());
    EXPECT_EQ(sodium::ExprKind::INTEGER_LITERAL, integer_literal_expr->expr_kind());
    EXPECT_EQ(2, integer_literal_expr->value());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

/*
    tests integer literal expression:
        567
*/
TEST(ParseExprTest, ParserCorrectlyParsesAMultiDigitIntegerLiteral) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("567");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto integer_literal_expr = parser.parse_integer_literal_expr();

    EXPECT_EQ(sodium::ASTNodeKind::EXPR, integer_literal_expr->node_kind());
    EXPECT_EQ(sodium::ExprKind::INTEGER_LITERAL, integer_literal_expr->expr_kind());
    EXPECT_EQ(567, integer_literal_expr->value());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

/*
    tests integer literal expression:
        func
*/
TEST(ParseExprTest, ParserDiagnosesParserErrorForInvalidIntegerLiteralExpression) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("func");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto integer_literal_expr = parser.parse_integer_literal_expr();

    EXPECT_EQ(nullptr, integer_literal_expr);

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_INTEGER_LITERAL, parser_error->kind());
}
