#include "sodium/nac/lexer/lexer.h"

#include <string_view>

#include <gtest/gtest.h>

#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/token/token.h"

TEST(LexerTest, LexerReadsFuncKeyword) {
    std::string_view func_keyword_string("func");
    sodium::Lexer lexer(func_keyword_string);
    sodium::Token token = lexer.get_next_token();

    EXPECT_EQ(sodium::TokenKind::KEYWORD_FUNC, token.kind());
    EXPECT_EQ(func_keyword_string, token.value());
}

TEST(LexerTest, LexerReadsReturnKeyword) {
    std::string_view return_keyword_string("return");
    sodium::Lexer lexer(return_keyword_string);
    sodium::Token token = lexer.get_next_token();

    EXPECT_EQ(sodium::TokenKind::KEYWORD_RETURN, token.kind());
    EXPECT_EQ(return_keyword_string, token.value());
}

TEST(LexerTest, LexerReadsIntType) {
    std::string_view int_type_string("int");
    sodium::Lexer lexer(int_type_string);
    sodium::Token token = lexer.get_next_token();

    EXPECT_EQ(sodium::TokenKind::TYPE, token.kind());
    EXPECT_EQ(int_type_string, token.value());
}

TEST(LexerTest, LexerReadsValidIdentifier) {
    std::string_view identifier_string("identifier");
    sodium::Lexer lexer(identifier_string);
    sodium::Token token = lexer.get_next_token();

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token.kind());
    EXPECT_EQ(identifier_string, token.value());
}

TEST(LexerTest, LexerReadsValidIdentifierWithUnderscorePrefix) {
    std::string_view identifier_string("_identifier");
    sodium::Lexer lexer(identifier_string);
    sodium::Token token = lexer.get_next_token();

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token.kind());
    EXPECT_EQ(identifier_string, token.value());
}

TEST(LexerTest, LexerReadsNumericLiteral) {
    std::string_view numeric_literal_string("2");
    sodium::Lexer lexer(numeric_literal_string);
    sodium::Token token = lexer.get_next_token();

    EXPECT_EQ(sodium::TokenKind::NUMERIC_LITERAL, token.kind());
    EXPECT_EQ(numeric_literal_string, token.value());
}

TEST(LexerTest, LexerReadsNumericLiteralWithMultipleDigits) {
    std::string_view numeric_literal_string("123456789");
    sodium::Lexer lexer(numeric_literal_string);
    sodium::Token token = lexer.get_next_token();

    EXPECT_EQ(sodium::TokenKind::NUMERIC_LITERAL, token.kind());
    EXPECT_EQ(numeric_literal_string, token.value());
}

TEST(LexerTest, LexerReadsLeftBrace) {
    std::string_view left_brace_string("{");
    sodium::Lexer lexer(left_brace_string);
    sodium::Token token = lexer.get_next_token();

    EXPECT_EQ(sodium::TokenKind::LEFT_BRACE, token.kind());
    EXPECT_EQ(left_brace_string, token.value());
}

TEST(LexerTest, LexerReadsRightBrace) {
    std::string_view right_brace_string("}");
    sodium::Lexer lexer(right_brace_string);
    sodium::Token token = lexer.get_next_token();

    EXPECT_EQ(sodium::TokenKind::RIGHT_BRACE, token.kind());
    EXPECT_EQ(right_brace_string, token.value());
}

TEST(LexerTest, LexerReadsLeftParen) {
    std::string_view leftParen_string("(");
    sodium::Lexer lexer(leftParen_string);
    sodium::Token token = lexer.get_next_token();

    EXPECT_EQ(sodium::TokenKind::LEFT_PAREN, token.kind());
    EXPECT_EQ(leftParen_string, token.value());
}

TEST(LexerTest, LexerReadsRightParen) {
    std::string_view rightParen_string(")");
    sodium::Lexer lexer(rightParen_string);
    sodium::Token token = lexer.get_next_token();

    EXPECT_EQ(sodium::TokenKind::RIGHT_PAREN, token.kind());
    EXPECT_EQ(rightParen_string, token.value());
}

TEST(LexerTest, LexerReadsSemicolon) {
    std::string_view semicolon_string(";");
    sodium::Lexer lexer(semicolon_string);
    sodium::Token token = lexer.get_next_token();

    EXPECT_EQ(sodium::TokenKind::SEMICOLON, token.kind());
    EXPECT_EQ(semicolon_string, token.value());
}

TEST(LexerTest, LexerReadsArrow) {
    std::string_view arrow_string("->");
    sodium::Lexer lexer(arrow_string);
    sodium::Token token = lexer.get_next_token();

    EXPECT_EQ(sodium::TokenKind::ARROW, token.kind());
    EXPECT_EQ(arrow_string, token.value());
}

TEST(LexerTest, LexerReadsEmpty_string) {
    std::string_view empty_string("");
    sodium::Lexer lexer(empty_string);
    sodium::Token token = lexer.get_next_token();

    EXPECT_EQ(sodium::TokenKind::EOF_TOKEN, token.kind());
    EXPECT_EQ(empty_string, token.value());
}

TEST(LexerTest, LexerSkipsWhitespace1) {
    sodium::Lexer lexer("  \n  \t \r\f \v   ");
    sodium::Token token = lexer.get_next_token();

    EXPECT_EQ(sodium::TokenKind::EOF_TOKEN, token.kind());
    EXPECT_EQ("", token.value());
}

TEST(LexerTest, LexerSkipsWhitespace2) {
    sodium::Lexer lexer("   \n\n \t \ridentifier\f \v   ");
    sodium::Token token = lexer.get_next_token();

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token.kind());
    EXPECT_EQ("identifier", token.value());
}

TEST(LexerTest, LexerRejectsInvalidIdentifier1) {
    sodium::Lexer lexer("$identifier");
    sodium::Token token = lexer.get_next_token();

    EXPECT_TRUE(sodium::ErrorManager::has_errors());
}

TEST(LexerTest, LexerRejectsInvalidIdentifier2) {
    sodium::Lexer lexer("ident$ifier");
    sodium::Token token = lexer.get_next_token();

    EXPECT_TRUE(sodium::ErrorManager::has_errors());
}

TEST(LexerTest, LexerRejectsInvalidToken) {
    sodium::Lexer lexer("$");
    sodium::Token token = lexer.get_next_token();

    EXPECT_TRUE(sodium::ErrorManager::has_errors());
}
