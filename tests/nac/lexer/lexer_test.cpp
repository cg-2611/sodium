#include "sodium/nac/lexer/lexer.h"

#include "gtest/gtest.h"

#include "sodium/nac/exceptions/exception.h"

TEST(LexerTest, LexerReadsEmptyString) {
    std::string emptyString("");
    nac::Lexer lexer(emptyString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(1, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_EOF, token.kind());
    EXPECT_EQ(emptyString, token.value());
}

TEST(LexerTest, LexerReadsValidIdentifier) {
    std::string identifierString("identifier");
    nac::Lexer lexer(identifierString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_IDENTIFIER, token.kind());
    EXPECT_EQ(identifierString, token.value());
}

TEST(LexerTest, LexerReadsNumericLiteral) {
    std::string numericLiteralString("2");
    nac::Lexer lexer(numericLiteralString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_NUMERIC_LITERAL, token.kind());
    EXPECT_EQ(numericLiteralString, token.value());
}

TEST(LexerTest, LexerReadsColon) {
    std::string colonString(":");
    nac::Lexer lexer(colonString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_COLON, token.kind());
    EXPECT_EQ(colonString, token.value());
}

TEST(LexerTest, LexerReadsLeftBrace) {
    std::string leftBraceString("{");
    nac::Lexer lexer(leftBraceString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_LEFT_BRACE, token.kind());
    EXPECT_EQ(leftBraceString, token.value());
}

TEST(LexerTest, LexerReadsLeftParen) {
    std::string leftParenString("(");
    nac::Lexer lexer(leftParenString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_LEFT_PAREN, token.kind());
    EXPECT_EQ(leftParenString, token.value());
}

TEST(LexerTest, LexerReadsRightBrace) {
    std::string rightBraceString("}");
    nac::Lexer lexer(rightBraceString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_RIGHT_BRACE, token.kind());
    EXPECT_EQ(rightBraceString, token.value());
}

TEST(LexerTest, LexerReadsRightParen) {
    std::string rightParenString(")");
    nac::Lexer lexer(rightParenString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_RIGHT_PAREN, token.kind());
    EXPECT_EQ(rightParenString, token.value());
}

TEST(LexerTest, LexerReadsSemiColon) {
    std::string semiColonString(";");
    nac::Lexer lexer(semiColonString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_SEMI_COLON, token.kind());
    EXPECT_EQ(semiColonString, token.value());
}

TEST(LexerTest, LexerReadsAllKeywords) {
    for (auto &&keyword : nac::KEYWORDS) {
        nac::Lexer lexer(keyword);
        std::vector<nac::Token> tokens = lexer.tokenize();

        ASSERT_EQ(2, tokens.size());

        nac::Token &token = tokens[0];

        EXPECT_EQ(nac::TokenKind::TOKEN_KEYWORD, token.kind());
        EXPECT_EQ(keyword, token.value());
    }
}

TEST(LexerTest, LexerReadsAllTypes) {
    for (auto &&type : nac::TYPES) {
        nac::Lexer lexer(type);
        std::vector<nac::Token> tokens = lexer.tokenize();

        ASSERT_EQ(2, tokens.size());

        nac::Token &token = tokens[0];

        EXPECT_EQ(nac::TokenKind::TOKEN_TYPE, token.kind());
        EXPECT_EQ(type, token.value());
    }
}

TEST(LexerTest, LexerReadsValidIdentifierWithUnderscorePrefix) {
    std::string identifierString("_identifier");
    nac::Lexer lexer(identifierString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_IDENTIFIER, token.kind());
    EXPECT_EQ(identifierString, token.value());
}

TEST(LexerTest, LexerReadsValidIdentifierWith$Prefix) {
    std::string identifierString("$identifier");
    nac::Lexer lexer(identifierString);
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_IDENTIFIER, token.kind());
    EXPECT_EQ(identifierString, token.value());
}

TEST(LexerTest, LexerReadsMultipleTokensFromAString) {
    nac::Lexer lexer("identifier 2;");
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(4, tokens.size());

    nac::Token &token1 = tokens[0];
    nac::Token &token2 = tokens[1];
    nac::Token &token3 = tokens[2];

    EXPECT_EQ(nac::TokenKind::TOKEN_IDENTIFIER, token1.kind());
    EXPECT_EQ("identifier", token1.value());

    EXPECT_EQ(nac::TokenKind::TOKEN_NUMERIC_LITERAL, token2.kind());
    EXPECT_EQ("2", token2.value());

    EXPECT_EQ(nac::TokenKind::TOKEN_SEMI_COLON, token3.kind());
    EXPECT_EQ(";", token3.value());
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

    EXPECT_EQ(nac::TokenKind::TOKEN_EOF, token.kind());
    EXPECT_EQ("", token.value());
}

TEST(LexerTest, LexerSkipsWhitespace2) {
    nac::Lexer lexer("  \n    \t \ridentifier\f \v   ");
    std::vector<nac::Token> tokens = lexer.tokenize();

    ASSERT_EQ(2, tokens.size());

    nac::Token &token = tokens[0];

    EXPECT_EQ(nac::TokenKind::TOKEN_IDENTIFIER, token.kind());
    EXPECT_EQ("identifier", token.value());
}
