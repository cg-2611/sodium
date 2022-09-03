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
    std::string_view literalExpressionString("2");

    auto literalExpression = std::make_unique<sodium::Token>(
        sodium::TokenKind::NUMERIC_LITERAL, literalExpressionString.data(), literalExpressionString.size(), 0, 0);

    sodium::Parser parser(literalExpression.get());
    auto literalExpr(parser.parseExpr());

    EXPECT_EQ(sodium::ASTNodeKind::EXPR, literalExpr->nodeKind());
    EXPECT_EQ(sodium::ExprKind::LITERAL, literalExpr->exprKind());
}

/*
    tests literal expression:
        2
*/
TEST(ParseExprTest, ParserCorrectlyDispatchesToParseANumericLiteralExpression) {
    std::string_view numericLiteralExpressionString("2");

    auto numericLiteralExpression =
        std::make_unique<sodium::Token>(sodium::TokenKind::NUMERIC_LITERAL, numericLiteralExpressionString.data(),
                                        numericLiteralExpressionString.size(), 0, 0);

    sodium::Parser parser(numericLiteralExpression.get());
    auto numericLiteralExpr(parser.parseLiteralExpr());

    EXPECT_EQ(sodium::ASTNodeKind::EXPR, numericLiteralExpr->nodeKind());
    EXPECT_EQ(sodium::ExprKind::LITERAL, numericLiteralExpr->exprKind());
    EXPECT_EQ(sodium::LiteralKind::NUMERIC_LITERAL, numericLiteralExpr->literalKind());
}

/*
    tests numeric literal expression:
        2
*/
TEST(ParseExprTest, ParserCorrectlyParsesASingleDigitNumericLiteral) {
    std::string_view numericLiteralExpressionString("2");

    auto numericLiteralExpression =
        std::make_unique<sodium::Token>(sodium::TokenKind::NUMERIC_LITERAL, numericLiteralExpressionString.data(),
                                        numericLiteralExpressionString.size(), 0, 0);

    sodium::Parser parser(numericLiteralExpression.get());
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

    auto numericLiteralExpression =
        std::make_unique<sodium::Token>(sodium::TokenKind::NUMERIC_LITERAL, numericLiteralExpressionString.data(),
                                        numericLiteralExpressionString.size(), 0, 0);

    sodium::Parser parser(numericLiteralExpression.get());
    auto numericLiteralExpr(parser.parseNumericLiteralExpr());

    EXPECT_EQ(sodium::ASTNodeKind::EXPR, numericLiteralExpr->nodeKind());
    EXPECT_EQ(sodium::ExprKind::LITERAL, numericLiteralExpr->exprKind());
    EXPECT_EQ(sodium::LiteralKind::NUMERIC_LITERAL, numericLiteralExpr->literalKind());

    EXPECT_EQ(567, numericLiteralExpr->value());
}
