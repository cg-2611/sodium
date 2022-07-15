#include "sodium/nac/lexer/lexer.h"

#include <vector>

#include <gtest/gtest.h>

#include "sodium/nac/error/error.h"

static const std::vector<std::string> tokensToTest = {
    "", "identifier", "2", ":", "{", "(", "}", ")", ";"
};

static int testTokenIndex;
static std::string stringToTokenize;

class LexerTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        testTokenIndex = 0;
        stringToTokenize = tokensToTest[testTokenIndex];
    }

    void SetUp() override {
        if (testTokenIndex < tokensToTest.size()) {
            stringToTokenize = tokensToTest[testTokenIndex];
        }

    }

    void TearDown() override {
        if (testTokenIndex < tokensToTest.size()) {
            testTokenIndex++;
        }
    }
};

TEST_F(LexerTest, LexerReadsEmptyString) {
    sodium::nac::Lexer lexer = sodium::nac::Lexer(stringToTokenize);
    sodium::nac::Token token = *(lexer.tokenize());

    EXPECT_EQ(sodium::nac::TokenKind::TOKEN_EOF, token.getKind());
    EXPECT_EQ(stringToTokenize, token.getValue());
}

TEST_F(LexerTest, LexerReadsValidIdentifier) {
    sodium::nac::Lexer lexer = sodium::nac::Lexer(stringToTokenize);
    sodium::nac::Token token = *(lexer.tokenize());

    EXPECT_EQ(sodium::nac::TokenKind::TOKEN_IDENTIFIER, token.getKind());
    EXPECT_EQ(stringToTokenize, token.getValue());
}


TEST_F(LexerTest, LexerReadsNumericLiteral) {
    sodium::nac::Lexer lexer = sodium::nac::Lexer(stringToTokenize);
    sodium::nac::Token token = *(lexer.tokenize());

    EXPECT_EQ(sodium::nac::TokenKind::TOKEN_NUMERIC_LITERAL, token.getKind());
    EXPECT_EQ(stringToTokenize, token.getValue());
}

TEST_F(LexerTest, LexerReadsColon) {
    sodium::nac::Lexer lexer = sodium::nac::Lexer(stringToTokenize);
    sodium::nac::Token token = *(lexer.tokenize());

    EXPECT_EQ(sodium::nac::TokenKind::TOKEN_COLON, token.getKind());
    EXPECT_EQ(stringToTokenize, token.getValue());
}

TEST_F(LexerTest, LexerReadsLeftBrace) {
    sodium::nac::Lexer lexer = sodium::nac::Lexer(stringToTokenize);
    sodium::nac::Token token = *(lexer.tokenize());

    EXPECT_EQ(sodium::nac::TokenKind::TOKEN_LEFT_BRACE, token.getKind());
    EXPECT_EQ(stringToTokenize, token.getValue());
}

TEST_F(LexerTest, LexerReadsLeftParen) {
    sodium::nac::Lexer lexer = sodium::nac::Lexer(stringToTokenize);
    sodium::nac::Token token = *(lexer.tokenize());

    EXPECT_EQ(sodium::nac::TokenKind::TOKEN_LEFT_PAREN, token.getKind());
    EXPECT_EQ(stringToTokenize, token.getValue());
}

TEST_F(LexerTest, LexerReadsRightBrace) {
    sodium::nac::Lexer lexer = sodium::nac::Lexer(stringToTokenize);
    sodium::nac::Token token = *(lexer.tokenize());

    EXPECT_EQ(sodium::nac::TokenKind::TOKEN_RIGHT_BRACE, token.getKind());
    EXPECT_EQ(stringToTokenize, token.getValue());
}

TEST_F(LexerTest, LexerReadsRightParen) {
    sodium::nac::Lexer lexer = sodium::nac::Lexer(stringToTokenize);
    sodium::nac::Token token = *(lexer.tokenize());

    EXPECT_EQ(sodium::nac::TokenKind::TOKEN_RIGHT_PAREN, token.getKind());
    EXPECT_EQ(stringToTokenize, token.getValue());
}

TEST_F(LexerTest, LexerReadsSemiColon) {
    sodium::nac::Lexer lexer = sodium::nac::Lexer(stringToTokenize);
    sodium::nac::Token token = *(lexer.tokenize());

    EXPECT_EQ(sodium::nac::TokenKind::TOKEN_SEMI_COLON, token.getKind());
    EXPECT_EQ(stringToTokenize, token.getValue());
}

TEST_F(LexerTest, LexerReadsAllKeywords) {
    for (auto &&keyword : sodium::nac::Token::KEYWORDS) {
        sodium::nac::Lexer lexer = sodium::nac::Lexer(keyword);
        sodium::nac::Token token = *(lexer.tokenize());

        EXPECT_EQ(sodium::nac::TokenKind::TOKEN_KEYWORD, token.getKind());
        EXPECT_EQ(keyword, token.getValue());
    }
}

TEST_F(LexerTest, LexerReadsAllTypes) {
    for (auto &&type : sodium::nac::Token::TYPES) {
        sodium::nac::Lexer lexer = sodium::nac::Lexer(type);
        sodium::nac::Token token = *(lexer.tokenize());

        EXPECT_EQ(sodium::nac::TokenKind::TOKEN_TYPE, token.getKind());
        EXPECT_EQ(type, token.getValue());
    }
}

TEST_F(LexerTest, LexerReadsValidIdentifierWithUnderscorePrefix) {
    std::string identifierToTest = "_identifier";
    sodium::nac::Lexer lexer = sodium::nac::Lexer(identifierToTest);
    sodium::nac::Token token = *(lexer.tokenize());

    EXPECT_EQ(sodium::nac::TokenKind::TOKEN_IDENTIFIER, token.getKind());
    EXPECT_EQ(identifierToTest, token.getValue());
}

TEST_F(LexerTest, LexerReadsValidIdentifierWith$Prefix) {
    std::string identifierToTest = "$identifier";
    sodium::nac::Lexer lexer = sodium::nac::Lexer(identifierToTest);
    sodium::nac::Token token = *(lexer.tokenize());

    EXPECT_EQ(sodium::nac::TokenKind::TOKEN_IDENTIFIER, token.getKind());
    EXPECT_EQ(identifierToTest, token.getValue());
}

TEST_F(LexerTest, LexerRejectsInvalidIdentifier1) {
    sodium::nac::Lexer lexer = sodium::nac::Lexer("§identifier");

    EXPECT_THROW(
        sodium::nac::Token token = *(lexer.tokenize()),
        sodium::nac::NACException
    );
}

TEST_F(LexerTest, LexerRejectsInvalidIdentifier2) {
    sodium::nac::Lexer lexer = sodium::nac::Lexer("ident§ifier");

    EXPECT_THROW(
        sodium::nac::Token token = *(lexer.tokenize()),
        sodium::nac::NACException
    );
}

TEST_F(LexerTest, LexerRejectsInvalidToken) {
    sodium::nac::Lexer lexer = sodium::nac::Lexer("§");

    EXPECT_THROW(
        sodium::nac::Token token = *(lexer.tokenize()),
        sodium::nac::NACException
    );
}
