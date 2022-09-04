#include "sodium/nac/lexer/lexer.h"

#include <vector>

#include <gtest/gtest.h>

#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/lexer/token.h"

TEST(LexerTest, LexerReadsAllKeywords) {
    for (auto &&keyword : sodium::KEYWORDS) {
        sodium::Lexer lexer(keyword);
        sodium::Token token = lexer.getNextToken();

        EXPECT_EQ(sodium::TokenKind::KEYWORD, token.kind());
        EXPECT_EQ(keyword, token.value());
    }
}

TEST(LexerTest, LexerReadsAllTypes) {
    for (auto &&type : sodium::TYPES) {
        sodium::Lexer lexer(type);
        sodium::Token token = lexer.getNextToken();

        EXPECT_EQ(sodium::TokenKind::TYPE, token.kind());
        EXPECT_EQ(type, token.value());
    }
}

TEST(LexerTest, LexerReadsValidIdentifier) {
    std::string identifierString("identifier");
    sodium::Lexer lexer(identifierString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token.kind());
    EXPECT_EQ(identifierString, token.value());
}

TEST(LexerTest, LexerReadsValidIdentifierWithUnderscorePrefix) {
    std::string identifierString("_identifier");
    sodium::Lexer lexer(identifierString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token.kind());
    EXPECT_EQ(identifierString, token.value());
}

TEST(LexerTest, LexerReadsNumericLiteral) {
    std::string numericLiteralString("2");
    sodium::Lexer lexer(numericLiteralString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::NUMERIC_LITERAL, token.kind());
    EXPECT_EQ(numericLiteralString, token.value());
}

TEST(LexerTest, LexerReadsNumericLiteralWithMultipleDigits) {
    std::string numericLiteralString("123456789");
    sodium::Lexer lexer(numericLiteralString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::NUMERIC_LITERAL, token.kind());
    EXPECT_EQ(numericLiteralString, token.value());
}

TEST(LexerTest, LexerReadsLeftBrace) {
    std::string leftBraceString("{");
    sodium::Lexer lexer(leftBraceString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::LEFT_BRACE, token.kind());
    EXPECT_EQ(leftBraceString, token.value());
}

TEST(LexerTest, LexerReadsRightBrace) {
    std::string rightBraceString("}");
    sodium::Lexer lexer(rightBraceString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::RIGHT_BRACE, token.kind());
    EXPECT_EQ(rightBraceString, token.value());
}

TEST(LexerTest, LexerReadsLeftParen) {
    std::string leftParenString("(");
    sodium::Lexer lexer(leftParenString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::LEFT_PAREN, token.kind());
    EXPECT_EQ(leftParenString, token.value());
}

TEST(LexerTest, LexerReadsRightParen) {
    std::string rightParenString(")");
    sodium::Lexer lexer(rightParenString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::RIGHT_PAREN, token.kind());
    EXPECT_EQ(rightParenString, token.value());
}

TEST(LexerTest, LexerReadsSemicolon) {
    std::string semicolonString(";");
    sodium::Lexer lexer(semicolonString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::SEMICOLON, token.kind());
    EXPECT_EQ(semicolonString, token.value());
}

TEST(LexerTest, LexerReadsArrow) {
    std::string arrowString("->");
    sodium::Lexer lexer(arrowString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::ARROW, token.kind());
    EXPECT_EQ(arrowString, token.value());
}

TEST(LexerTest, LexerReadsEmptyString) {
    std::string emptyString;
    sodium::Lexer lexer(emptyString);
    sodium::Token token = lexer.getNextToken();

    EXPECT_EQ(sodium::TokenKind::EOF_TOKEN, token.kind());
    EXPECT_EQ(emptyString, token.value());
}

TEST(LexerTest, LexerReadsMultipleTokensFromAString) {
    sodium::Lexer lexer("identifier 2 { -> )");
    std::vector<sodium::Token> tokens = lexer.tokenize();

    ASSERT_EQ(6, tokens.size());

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, tokens[0].kind());
    EXPECT_EQ("identifier", tokens[0].value());

    EXPECT_EQ(sodium::TokenKind::NUMERIC_LITERAL, tokens[1].kind());
    EXPECT_EQ("2", tokens[1].value());

    EXPECT_EQ(sodium::TokenKind::LEFT_BRACE, tokens[2].kind());
    EXPECT_EQ("{", tokens[2].value());

    EXPECT_EQ(sodium::TokenKind::ARROW, tokens[3].kind());
    EXPECT_EQ("->", tokens[3].value());

    EXPECT_EQ(sodium::TokenKind::RIGHT_PAREN, tokens[4].kind());
    EXPECT_EQ(")", tokens[4].value());

    EXPECT_EQ(sodium::TokenKind::EOF_TOKEN, tokens[5].kind());
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
