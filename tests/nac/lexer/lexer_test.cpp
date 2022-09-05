#include "sodium/nac/lexer/lexer.h"

#include <string_view>

#include <gtest/gtest.h>

#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/lexer/token.h"

TEST(LexerTest, LexerReadsFuncKeyword) {
    std::string_view funcKeywordString("func");
    sodium::Lexer lexer(funcKeywordString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::KEYWORD_FUNC, token.kind());
    EXPECT_EQ(funcKeywordString, token.value());
}

TEST(LexerTest, LexerReadsReturnKeyword) {
    std::string_view returnKeywordString("return");
    sodium::Lexer lexer(returnKeywordString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::KEYWORD_RETURN, token.kind());
    EXPECT_EQ(returnKeywordString, token.value());
}

TEST(LexerTest, LexerReadsIntType) {
    std::string_view intTypeString("int");
    sodium::Lexer lexer(intTypeString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::TYPE, token.kind());
    EXPECT_EQ(intTypeString, token.value());
}

TEST(LexerTest, LexerReadsValidIdentifier) {
    std::string_view identifierString("identifier");
    sodium::Lexer lexer(identifierString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token.kind());
    EXPECT_EQ(identifierString, token.value());
}

TEST(LexerTest, LexerReadsValidIdentifierWithUnderscorePrefix) {
    std::string_view identifierString("_identifier");
    sodium::Lexer lexer(identifierString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token.kind());
    EXPECT_EQ(identifierString, token.value());
}

TEST(LexerTest, LexerReadsNumericLiteral) {
    std::string_view numericLiteralString("2");
    sodium::Lexer lexer(numericLiteralString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::NUMERIC_LITERAL, token.kind());
    EXPECT_EQ(numericLiteralString, token.value());
}

TEST(LexerTest, LexerReadsNumericLiteralWithMultipleDigits) {
    std::string_view numericLiteralString("123456789");
    sodium::Lexer lexer(numericLiteralString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::NUMERIC_LITERAL, token.kind());
    EXPECT_EQ(numericLiteralString, token.value());
}

TEST(LexerTest, LexerReadsLeftBrace) {
    std::string_view leftBraceString("{");
    sodium::Lexer lexer(leftBraceString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::LEFT_BRACE, token.kind());
    EXPECT_EQ(leftBraceString, token.value());
}

TEST(LexerTest, LexerReadsRightBrace) {
    std::string_view rightBraceString("}");
    sodium::Lexer lexer(rightBraceString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::RIGHT_BRACE, token.kind());
    EXPECT_EQ(rightBraceString, token.value());
}

TEST(LexerTest, LexerReadsLeftParen) {
    std::string_view leftParenString("(");
    sodium::Lexer lexer(leftParenString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::LEFT_PAREN, token.kind());
    EXPECT_EQ(leftParenString, token.value());
}

TEST(LexerTest, LexerReadsRightParen) {
    std::string_view rightParenString(")");
    sodium::Lexer lexer(rightParenString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::RIGHT_PAREN, token.kind());
    EXPECT_EQ(rightParenString, token.value());
}

TEST(LexerTest, LexerReadsSemicolon) {
    std::string_view semicolonString(";");
    sodium::Lexer lexer(semicolonString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::SEMICOLON, token.kind());
    EXPECT_EQ(semicolonString, token.value());
}

TEST(LexerTest, LexerReadsArrow) {
    std::string_view arrowString("->");
    sodium::Lexer lexer(arrowString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::ARROW, token.kind());
    EXPECT_EQ(arrowString, token.value());
}

TEST(LexerTest, LexerReadsEmptyString) {
    std::string_view emptyString("");
    sodium::Lexer lexer(emptyString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::EOF_TOKEN, token.kind());
    EXPECT_EQ(emptyString, token.value());
}

TEST(LexerTest, LexerSkipsWhitespace1) {
    sodium::Lexer lexer("  \n  \t \r\f \v   ");
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::EOF_TOKEN, token.kind());
    EXPECT_EQ("", token.value());
}

TEST(LexerTest, LexerSkipsWhitespace2) {
    sodium::Lexer lexer("   \n\n \t \ridentifier\f \v   ");
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token.kind());
    EXPECT_EQ("identifier", token.value());
}

TEST(LexerTest, LexerRejectsInvalidIdentifier1) {
    sodium::Lexer lexer("$identifier");
    sodium::Token token = lexer.getNextToken();

    EXPECT_TRUE(sodium::ErrorManager::hasErrors());
}

TEST(LexerTest, LexerRejectsInvalidIdentifier2) {
    sodium::Lexer lexer("ident$ifier");
    sodium::Token token = lexer.getNextToken();

    EXPECT_TRUE(sodium::ErrorManager::hasErrors());
}

TEST(LexerTest, LexerRejectsInvalidToken) {
    sodium::Lexer lexer("$");
    sodium::Token token = lexer.getNextToken();

    EXPECT_TRUE(sodium::ErrorManager::hasErrors());
}
