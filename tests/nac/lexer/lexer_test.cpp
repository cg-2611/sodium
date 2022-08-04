#include "sodium/nac/lexer/lexer.h"

#include <iostream>

#include <gtest/gtest.h>

#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/lexer/token.h"

TEST(LexerTest, LexerReadsAllKeywords) {
    for (auto &&keyword : nac::KEYWORDS) {
        nac::Lexer lexer(keyword);
        std::unique_ptr<nac::Token> token = lexer.tokenize();

        EXPECT_EQ(nac::TokenKind::KEYWORD, token->kind());
        EXPECT_EQ(keyword, token->value());
    }
}

TEST(LexerTest, LexerReadsAllTypes) {
    for (auto &&type : nac::TYPES) {
        nac::Lexer lexer(type);
        std::unique_ptr<nac::Token> token = lexer.tokenize();

        EXPECT_EQ(nac::TokenKind::TYPE, token->kind());
        EXPECT_EQ(type, token->value());
    }
}

TEST(LexerTest, LexerReadsValidIdentifier) {
    std::string identifierString("identifier");
    nac::Lexer lexer(identifierString);
    std::unique_ptr<nac::Token> token = lexer.tokenize();

    EXPECT_EQ(nac::TokenKind::IDENTIFIER, token->kind());
    EXPECT_EQ(identifierString, token->value());
}


TEST(LexerTest, LexerReadsValidIdentifierWithUnderscorePrefix) {
    std::string identifierString("_identifier");
    nac::Lexer lexer(identifierString);
    std::unique_ptr<nac::Token> token = lexer.tokenize();

    EXPECT_EQ(nac::TokenKind::IDENTIFIER, token->kind());
    EXPECT_EQ(identifierString, token->value());
}


TEST(LexerTest, LexerRejectsInvalidIdentifier1) {
    nac::Lexer lexer("$identifier");
    std::unique_ptr<nac::Token> token = lexer.tokenize();

    EXPECT_TRUE(nac::ErrorManager::hasErrors());
}

TEST(LexerTest, LexerRejectsInvalidIdentifier2) {
    nac::Lexer lexer("ident$ifier");
    std::unique_ptr<nac::Token> token = lexer.tokenize();

    EXPECT_TRUE(nac::ErrorManager::hasErrors());
}

TEST(LexerTest, LexerRejectsInvalidToken) {
    nac::Lexer lexer("$");
    std::unique_ptr<nac::Token> token = lexer.tokenize();

    EXPECT_TRUE(nac::ErrorManager::hasErrors());
}

TEST(LexerTest, LexerReadsNumericLiteral) {
    std::string numericLiteralString("2");
    nac::Lexer lexer(numericLiteralString);
    std::unique_ptr<nac::Token> token = lexer.tokenize();

    EXPECT_EQ(nac::TokenKind::NUMERIC_LITERAL, token->kind());
    EXPECT_EQ(numericLiteralString, token->value());
}

TEST(LexerTest, LexerReadsNumericLiteralWithMultipleDigits) {
    std::string numericLiteralString("123456789");
    nac::Lexer lexer(numericLiteralString);
    std::unique_ptr<nac::Token> token = lexer.tokenize();

    EXPECT_EQ(nac::TokenKind::NUMERIC_LITERAL, token->kind());
    EXPECT_EQ(numericLiteralString, token->value());
}

TEST(LexerTest, LexerReadsLeftBrace) {
    std::string leftBraceString("{");
    nac::Lexer lexer(leftBraceString);
    std::unique_ptr<nac::Token> token = lexer.tokenize();

    EXPECT_EQ(nac::TokenKind::LEFT_BRACE, token->kind());
    EXPECT_EQ(leftBraceString, token->value());
}

TEST(LexerTest, LexerReadsRightBrace) {
    std::string rightBraceString("}");
    nac::Lexer lexer(rightBraceString);
    std::unique_ptr<nac::Token> token = lexer.tokenize();

    EXPECT_EQ(nac::TokenKind::RIGHT_BRACE, token->kind());
    EXPECT_EQ(rightBraceString, token->value());
}

TEST(LexerTest, LexerReadsLeftParen) {
    std::string leftParenString("(");
    nac::Lexer lexer(leftParenString);
    std::unique_ptr<nac::Token> token = lexer.tokenize();

    EXPECT_EQ(nac::TokenKind::LEFT_PAREN, token->kind());
    EXPECT_EQ(leftParenString, token->value());
}

TEST(LexerTest, LexerReadsRightParen) {
    std::string rightParenString(")");
    nac::Lexer lexer(rightParenString);
    std::unique_ptr<nac::Token> token = lexer.tokenize();

    EXPECT_EQ(nac::TokenKind::RIGHT_PAREN, token->kind());
    EXPECT_EQ(rightParenString, token->value());
}

TEST(LexerTest, LexerReadsArrow) {
    std::string arrowString("->");
    nac::Lexer lexer(arrowString);
    std::unique_ptr<nac::Token> token = lexer.tokenize();

    EXPECT_EQ(nac::TokenKind::ARROW, token->kind());
    EXPECT_EQ(arrowString, token->value());
}

TEST(LexerTest, LexerReadsEmptyString) {
    std::string emptyString("");
    nac::Lexer lexer(emptyString);
    std::unique_ptr<nac::Token> token = lexer.tokenize();

    EXPECT_EQ(nac::TokenKind::EOF_TOKEN, token->kind());
    EXPECT_EQ(emptyString, token->value());
}

TEST(LexerTest, LexerReadsEndOfLine) {
    std::string endOfLineString("\n");
    nac::Lexer lexer(endOfLineString);
    std::unique_ptr<nac::Token> token = lexer.tokenize();

    EXPECT_EQ(nac::TokenKind::EOL_TOKEN, token->kind());
    EXPECT_EQ(endOfLineString, token->value());
}

TEST(LexerTest, LexerReadsMultipleTokensFromAString) {
    nac::Lexer lexer("identifier 2 { -> )");
    std::unique_ptr<nac::Token> firstToken = lexer.tokenize();

    nac::Token *token1 = firstToken.get();
    nac::Token *token2 = firstToken->next();
    nac::Token *token3 = firstToken->next()->next();
    nac::Token *token4 = firstToken->next()->next()->next();
    nac::Token *token5 = firstToken->next()->next()->next()->next();

    EXPECT_EQ(nac::TokenKind::IDENTIFIER, token1->kind());
    EXPECT_EQ("identifier", token1->value());

    EXPECT_EQ(nac::TokenKind::NUMERIC_LITERAL, token2->kind());
    EXPECT_EQ("2", token2->value());

    EXPECT_EQ(nac::TokenKind::LEFT_BRACE, token3->kind());
    EXPECT_EQ("{", token3->value());

    EXPECT_EQ(nac::TokenKind::ARROW, token4->kind());
    EXPECT_EQ("->", token4->value());

    EXPECT_EQ(nac::TokenKind::RIGHT_PAREN, token5->kind());
    EXPECT_EQ(")", token5->value());
}

TEST(LexerTest, LexerSkipsWhitespace1) {
    nac::Lexer lexer("    \t \r\f \v   ");
    std::unique_ptr<nac::Token> token = lexer.tokenize();

    EXPECT_EQ(nac::TokenKind::EOF_TOKEN, token->kind());
    EXPECT_EQ("", token->value());
}

TEST(LexerTest, LexerSkipsWhitespace2) {
    nac::Lexer lexer("    \t \ridentifier\f \v   ");
    std::unique_ptr<nac::Token> token = lexer.tokenize();

    EXPECT_EQ(nac::TokenKind::IDENTIFIER, token->kind());
    EXPECT_EQ("identifier", token->value());
}
