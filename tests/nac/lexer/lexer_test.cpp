#include "sodium/nac/lexer/lexer.h"

#include "gtest/gtest.h"

#include "sodium/nac/error/error.h"

TEST(LexerTest, LexerReadsEmptyString) {
    std::string emptyString("");
    nac::Lexer lexer(emptyString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(1, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_EOF, token.getKind());
    EXPECT_EQ(emptyString, token.getValue());
}

TEST(LexerTest, LexerReadsValidIdentifier) {
    std::string identifierString("identifier");
    nac::Lexer lexer(identifierString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_IDENTIFIER, token.getKind());
    EXPECT_EQ(identifierString, token.getValue());
}

TEST(LexerTest, LexerReadsNumericLiteral) {
    std::string numericLiteralString("2");
    nac::Lexer lexer(numericLiteralString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_NUMERIC_LITERAL, token.getKind());
    EXPECT_EQ(numericLiteralString, token.getValue());
}

TEST(LexerTest, LexerReadsColon) {
    std::string colonString(":");
    nac::Lexer lexer(colonString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_COLON, token.getKind());
    EXPECT_EQ(colonString, token.getValue());
}

TEST(LexerTest, LexerReadsLeftBrace) {
    std::string leftBraceString("{");
    nac::Lexer lexer(leftBraceString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_LEFT_BRACE, token.getKind());
    EXPECT_EQ(leftBraceString, token.getValue());
}

TEST(LexerTest, LexerReadsLeftParen) {
    std::string leftParenString("(");
    nac::Lexer lexer(leftParenString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_LEFT_PAREN, token.getKind());
    EXPECT_EQ(leftParenString, token.getValue());
}

TEST(LexerTest, LexerReadsRightBrace) {
    std::string rightBraceString("}");
    nac::Lexer lexer(rightBraceString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_RIGHT_BRACE, token.getKind());
    EXPECT_EQ(rightBraceString, token.getValue());
}

TEST(LexerTest, LexerReadsRightParen) {
    std::string rightParenString(")");
    nac::Lexer lexer(rightParenString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_RIGHT_PAREN, token.getKind());
    EXPECT_EQ(rightParenString, token.getValue());
}

TEST(LexerTest, LexerReadsSemiColon) {
    std::string semiColonString(";");
    nac::Lexer lexer(semiColonString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_SEMI_COLON, token.getKind());
    EXPECT_EQ(semiColonString, token.getValue());
}

TEST(LexerTest, LexerReadsAllKeywords) {
    for (auto &&keyword : nac::KEYWORDS) {
        nac::Lexer lexer(keyword);
        std::vector<nac::Token> tokens = lexer.tokenize();

        ASSERT_EQ(2, tokens.size());

        nac::Token &token = tokens[0];

        EXPECT_EQ(nac::TokenKind::TOKEN_KEYWORD, token.getKind());
        EXPECT_EQ(keyword, token.getValue());
    }
}

TEST(LexerTest, LexerReadsAllTypes) {
    for (auto &&type : nac::TYPES) {
        nac::Lexer lexer(type);
        std::vector<nac::Token> tokens = lexer.tokenize();

        ASSERT_EQ(2, tokens.size());

        nac::Token &token = tokens[0];

        EXPECT_EQ(nac::TokenKind::TOKEN_TYPE, token.getKind());
        EXPECT_EQ(type, token.getValue());
    }
}

TEST(LexerTest, LexerReadsValidIdentifierWithUnderscorePrefix) {
    std::string identifierString("_identifier");
    nac::Lexer lexer(identifierString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_IDENTIFIER, token.getKind());
    EXPECT_EQ(identifierString, token.getValue());
}

TEST(LexerTest, LexerReadsValidIdentifierWith$Prefix) {
    std::string identifierString("$identifier");
    nac::Lexer lexer(identifierString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_IDENTIFIER, token.getKind());
    EXPECT_EQ(identifierString, token.getValue());
}

TEST(LexerTest, LexerReadsMultipleTokensFromAString) {
    nac::Lexer lexer("identifier 2;");
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(4, tokens.size());

    nac::Token &token1 = tokens[0];
    nac::Token &token2 = tokens[1];
    nac::Token &token3 = tokens[2];

    EXPECT_EQ(nac::TokenKind::TOKEN_IDENTIFIER, token1.getKind());
    EXPECT_EQ("identifier", token1.getValue());

    EXPECT_EQ(nac::TokenKind::TOKEN_NUMERIC_LITERAL, token2.getKind());
    EXPECT_EQ("2", token2.getValue());

    EXPECT_EQ(nac::TokenKind::TOKEN_SEMI_COLON, token3.getKind());
    EXPECT_EQ(";", token3.getValue());
}

TEST(LexerTest, LexerRejectsInvalidIdentifier1) {
    nac::Lexer lexer("§identifier");

    EXPECT_THROW(
        lexer.tokenize(),
        nac::Exception
    );
}

TEST(LexerTest, LexerRejectsInvalidIdentifier2) {
    nac::Lexer lexer("ident§ifier");

    EXPECT_THROW(
        lexer.tokenize(),
        nac::Exception
    );
}

TEST(LexerTest, LexerRejectsInvalidToken) {
    nac::Lexer lexer("§");

    EXPECT_THROW(
        lexer.tokenize(),
        nac::Exception
    );
}

TEST(LexerTest, LexerSkipsWhitespace1) {
    nac::Lexer lexer("  \n    \t \r\f \v   ");
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(1, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_EOF, token.getKind());
    EXPECT_EQ("", token.getValue());
}

TEST(LexerTest, LexerSkipsWhitespace2) {
    nac::Lexer lexer("  \n    \t \ridentifier\f \v   ");
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_IDENTIFIER, token.getKind());
    EXPECT_EQ("identifier", token.getValue());
}
