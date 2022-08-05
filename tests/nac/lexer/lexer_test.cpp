#include "sodium/nac/lexer/lexer.h"

#include <iostream>

#include <gtest/gtest.h>

#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/lexer/token.h"

TEST(LexerTest, LexerReadsAllKeywords) {
    for (auto &&keyword : sodium::KEYWORDS) {
        sodium::Lexer lexer(keyword);
        std::unique_ptr<sodium::Token> token = lexer.tokenize();

        EXPECT_EQ(sodium::TokenKind::KEYWORD, token->kind());
        EXPECT_EQ(keyword, token->value());
    }
}

TEST(LexerTest, LexerReadsAllTypes) {
    for (auto &&type : sodium::TYPES) {
        sodium::Lexer lexer(type);
        std::unique_ptr<sodium::Token> token = lexer.tokenize();

        EXPECT_EQ(sodium::TokenKind::TYPE, token->kind());
        EXPECT_EQ(type, token->value());
    }
}

TEST(LexerTest, LexerReadsValidIdentifier) {
    std::string identifierString("identifier");
    sodium::Lexer lexer(identifierString);
    std::unique_ptr<sodium::Token> token = lexer.tokenize();

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token->kind());
    EXPECT_EQ(identifierString, token->value());
}


TEST(LexerTest, LexerReadsValidIdentifierWithUnderscorePrefix) {
    std::string identifierString("_identifier");
    sodium::Lexer lexer(identifierString);
    std::unique_ptr<sodium::Token> token = lexer.tokenize();

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token->kind());
    EXPECT_EQ(identifierString, token->value());
}


TEST(LexerTest, LexerRejectsInvalidIdentifier1) {
    sodium::Lexer lexer("$identifier");
    std::unique_ptr<sodium::Token> token = lexer.tokenize();

    EXPECT_TRUE(sodium::ErrorManager::hasErrors());
}

TEST(LexerTest, LexerRejectsInvalidIdentifier2) {
    sodium::Lexer lexer("ident$ifier");
    std::unique_ptr<sodium::Token> token = lexer.tokenize();

    EXPECT_TRUE(sodium::ErrorManager::hasErrors());
}

TEST(LexerTest, LexerRejectsInvalidToken) {
    sodium::Lexer lexer("$");
    std::unique_ptr<sodium::Token> token = lexer.tokenize();

    EXPECT_TRUE(sodium::ErrorManager::hasErrors());
}

TEST(LexerTest, LexerReadsNumericLiteral) {
    std::string numericLiteralString("2");
    sodium::Lexer lexer(numericLiteralString);
    std::unique_ptr<sodium::Token> token = lexer.tokenize();

    EXPECT_EQ(sodium::TokenKind::NUMERIC_LITERAL, token->kind());
    EXPECT_EQ(numericLiteralString, token->value());
}

TEST(LexerTest, LexerReadsNumericLiteralWithMultipleDigits) {
    std::string numericLiteralString("123456789");
    sodium::Lexer lexer(numericLiteralString);
    std::unique_ptr<sodium::Token> token = lexer.tokenize();

    EXPECT_EQ(sodium::TokenKind::NUMERIC_LITERAL, token->kind());
    EXPECT_EQ(numericLiteralString, token->value());
}

TEST(LexerTest, LexerReadsLeftBrace) {
    std::string leftBraceString("{");
    sodium::Lexer lexer(leftBraceString);
    std::unique_ptr<sodium::Token> token = lexer.tokenize();

    EXPECT_EQ(sodium::TokenKind::LEFT_BRACE, token->kind());
    EXPECT_EQ(leftBraceString, token->value());
}

TEST(LexerTest, LexerReadsRightBrace) {
    std::string rightBraceString("}");
    sodium::Lexer lexer(rightBraceString);
    std::unique_ptr<sodium::Token> token = lexer.tokenize();

    EXPECT_EQ(sodium::TokenKind::RIGHT_BRACE, token->kind());
    EXPECT_EQ(rightBraceString, token->value());
}

TEST(LexerTest, LexerReadsLeftParen) {
    std::string leftParenString("(");
    sodium::Lexer lexer(leftParenString);
    std::unique_ptr<sodium::Token> token = lexer.tokenize();

    EXPECT_EQ(sodium::TokenKind::LEFT_PAREN, token->kind());
    EXPECT_EQ(leftParenString, token->value());
}

TEST(LexerTest, LexerReadsRightParen) {
    std::string rightParenString(")");
    sodium::Lexer lexer(rightParenString);
    std::unique_ptr<sodium::Token> token = lexer.tokenize();

    EXPECT_EQ(sodium::TokenKind::RIGHT_PAREN, token->kind());
    EXPECT_EQ(rightParenString, token->value());
}

TEST(LexerTest, LexerReadsArrow) {
    std::string arrowString("->");
    sodium::Lexer lexer(arrowString);
    std::unique_ptr<sodium::Token> token = lexer.tokenize();

    EXPECT_EQ(sodium::TokenKind::ARROW, token->kind());
    EXPECT_EQ(arrowString, token->value());
}

TEST(LexerTest, LexerReadsEmptyString) {
    std::string emptyString("");
    sodium::Lexer lexer(emptyString);
    std::unique_ptr<sodium::Token> token = lexer.tokenize();

    EXPECT_EQ(sodium::TokenKind::EOF_TOKEN, token->kind());
    EXPECT_EQ(emptyString, token->value());
}

TEST(LexerTest, LexerReadsEndOfLine) {
    std::string endOfLineString("\n");
    sodium::Lexer lexer(endOfLineString);
    std::unique_ptr<sodium::Token> token = lexer.tokenize();

    EXPECT_EQ(sodium::TokenKind::EOL_TOKEN, token->kind());
    EXPECT_EQ(endOfLineString, token->value());
}

TEST(LexerTest, LexerReadsMultipleTokensFromAString) {
    sodium::Lexer lexer("identifier 2 { -> )");
    std::unique_ptr<sodium::Token> firstToken = lexer.tokenize();

    sodium::Token *token1 = firstToken.get();
    sodium::Token *token2 = firstToken->next();
    sodium::Token *token3 = firstToken->next()->next();
    sodium::Token *token4 = firstToken->next()->next()->next();
    sodium::Token *token5 = firstToken->next()->next()->next()->next();

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token1->kind());
    EXPECT_EQ("identifier", token1->value());

    EXPECT_EQ(sodium::TokenKind::NUMERIC_LITERAL, token2->kind());
    EXPECT_EQ("2", token2->value());

    EXPECT_EQ(sodium::TokenKind::LEFT_BRACE, token3->kind());
    EXPECT_EQ("{", token3->value());

    EXPECT_EQ(sodium::TokenKind::ARROW, token4->kind());
    EXPECT_EQ("->", token4->value());

    EXPECT_EQ(sodium::TokenKind::RIGHT_PAREN, token5->kind());
    EXPECT_EQ(")", token5->value());
}

TEST(LexerTest, LexerSkipsWhitespace1) {
    sodium::Lexer lexer("    \t \r\f \v   ");
    std::unique_ptr<sodium::Token> token = lexer.tokenize();

    EXPECT_EQ(sodium::TokenKind::EOF_TOKEN, token->kind());
    EXPECT_EQ("", token->value());
}

TEST(LexerTest, LexerSkipsWhitespace2) {
    sodium::Lexer lexer("    \t \ridentifier\f \v   ");
    std::unique_ptr<sodium::Token> token = lexer.tokenize();

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token->kind());
    EXPECT_EQ("identifier", token->value());
}
