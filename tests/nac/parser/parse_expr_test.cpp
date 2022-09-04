#include "sodium/nac/parser/parser.h"

#include <memory>
#include <string_view>
#include <utility>

#include <gtest/gtest.h>

#include "sodium/nac/ast/expr.h"
#include "sodium/nac/lexer/token.h"

/*
    tests expression:
        2
*/
TEST(ParseExprTest, ParserCorrectlyDispatchesToParseALiteralExpression) {
    std::string_view expressionString("2");

    sodium::Parser parser(expressionString);
    auto expr(parser.parseExpr());

    EXPECT_EQ(sodium::ASTNodeKind::EXPR, expr->nodeKind());
    EXPECT_EQ(sodium::ExprKind::LITERAL, expr->exprKind());
}

/*
    tests literal expression:
        2
*/
TEST(ParseExprTest, ParserCorrectlyDispatchesToParseANumericLiteralExpression) {
    std::string_view literalExpressionString("2");

    sodium::Parser parser(literalExpressionString);
    auto literalExpr(parser.parseLiteralExpr());

    EXPECT_EQ(sodium::ASTNodeKind::EXPR, literalExpr->nodeKind());
    EXPECT_EQ(sodium::ExprKind::LITERAL, literalExpr->exprKind());
    EXPECT_EQ(sodium::LiteralKind::NUMERIC_LITERAL, literalExpr->literalKind());
}

/*
    tests numeric literal expression:
        2
*/
TEST(ParseExprTest, ParserCorrectlyParsesASingleDigitNumericLiteral) {
    std::string_view numericLiteralExpressionString("2");

    sodium::Parser parser(numericLiteralExpressionString);
    auto numericLiteralExpr(parser.parseNumericLiteralExpr());

    EXPECT_EQ(sodium::ASTNodeKind::EXPR, numericLiteralExpr->nodeKind());
    EXPECT_EQ(sodium::ExprKind::LITERAL, numericLiteralExpr->exprKind());
    EXPECT_EQ(sodium::LiteralKind::NUMERIC_LITERAL, numericLiteralExpr->literalKind());

    EXPECT_EQ(2, numericLiteralExpr->value());
}

/*
    tests numeric literal expression:
        567
*/
TEST(ParseExprTest, ParserCorrectlyParsesAMultiDigitNumericLiteral) {
    std::string_view numericLiteralExpressionString("567");

    sodium::Parser parser(numericLiteralExpressionString);
    auto numericLiteralExpr(parser.parseNumericLiteralExpr());

    EXPECT_EQ(sodium::ASTNodeKind::EXPR, numericLiteralExpr->nodeKind());
    EXPECT_EQ(sodium::ExprKind::LITERAL, numericLiteralExpr->exprKind());
    EXPECT_EQ(sodium::LiteralKind::NUMERIC_LITERAL, numericLiteralExpr->literalKind());

    EXPECT_EQ(567, numericLiteralExpr->value());
}
