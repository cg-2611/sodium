#include "sodium/nac/lexer/lexer.h"

#include <string_view>

#include "gtest/gtest.h"

#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/token/token.h"
#include "sodium/nac/token/token_kind.h"

TEST(LexerTest, LexerReadsFuncKeyword) {
    auto func_keyword_string = std::string_view("func");
    auto lexer = sodium::Lexer(func_keyword_string);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::KEYWORD_FUNC, token.kind());
    EXPECT_EQ(func_keyword_string, token.value());
}

TEST(LexerTest, LexerReadsReturnKeyword) {
    auto return_keyword_string = std::string_view("return");
    auto lexer = sodium::Lexer(return_keyword_string);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::KEYWORD_RETURN, token.kind());
    EXPECT_EQ(return_keyword_string, token.value());
}

TEST(LexerTest, LexerReadsIntType) {
    auto int_type_string = std::string_view("int");
    auto lexer = sodium::Lexer(int_type_string);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::TYPE, token.kind());
    EXPECT_EQ(int_type_string, token.value());
}

TEST(LexerTest, LexerReadsValidIdentifier) {
    auto identifier_string = std::string_view("identifier");
    auto lexer = sodium::Lexer(identifier_string);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token.kind());
    EXPECT_EQ(identifier_string, token.value());
}

TEST(LexerTest, LexerReadsValidIdentifierWithUnderscorePrefix) {
    auto identifier_string = std::string_view("_identifier");
    auto lexer = sodium::Lexer(identifier_string);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token.kind());
    EXPECT_EQ(identifier_string, token.value());
}

TEST(LexerTest, LexerReadsIntegerLiteral) {
    auto integer_literal_string = std::string_view("2");
    auto lexer = sodium::Lexer(integer_literal_string);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::INTEGER_LITERAL, token.kind());
    EXPECT_EQ(integer_literal_string, token.value());
}

TEST(LexerTest, LexerReadsIntegerLiteralWithMultipleDigits) {
    auto integer_literal_string = std::string_view("123456789");
    auto lexer = sodium::Lexer(integer_literal_string);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::INTEGER_LITERAL, token.kind());
    EXPECT_EQ(integer_literal_string, token.value());
}

TEST(LexerTest, LexerReadsLeftBrace) {
    auto left_brace_string = std::string_view("{");
    auto lexer = sodium::Lexer(left_brace_string);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::LEFT_BRACE, token.kind());
    EXPECT_EQ(left_brace_string, token.value());
}

TEST(LexerTest, LexerReadsRightBrace) {
    auto right_brace_string = std::string_view("}");
    auto lexer = sodium::Lexer(right_brace_string);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::RIGHT_BRACE, token.kind());
    EXPECT_EQ(right_brace_string, token.value());
}

TEST(LexerTest, LexerReadsLeftParen) {
    auto left_paren_string = std::string_view("(");
    auto lexer = sodium::Lexer(left_paren_string);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::LEFT_PAREN, token.kind());
    EXPECT_EQ(left_paren_string, token.value());
}

TEST(LexerTest, LexerReadsRightParen) {
    auto right_paren_string = std::string_view(")");
    auto lexer = sodium::Lexer(right_paren_string);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::RIGHT_PAREN, token.kind());
    EXPECT_EQ(right_paren_string, token.value());
}

TEST(LexerTest, LexerReadsSemicolon) {
    auto semicolon_string = std::string_view(";");
    auto lexer = sodium::Lexer(semicolon_string);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::SEMICOLON, token.kind());
    EXPECT_EQ(semicolon_string, token.value());
}

TEST(LexerTest, LexerReadsArrow) {
    auto arrow_string = std::string_view("->");
    auto lexer = sodium::Lexer(arrow_string);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::ARROW, token.kind());
    EXPECT_EQ(arrow_string, token.value());
}

TEST(LexerTest, LexerReadsEmpty_string) {
    auto empty_string = std::string_view("");
    auto lexer = sodium::Lexer(empty_string);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::EOF_TOKEN, token.kind());
    EXPECT_EQ(empty_string, token.value());
}

TEST(LexerTest, LexerSkipsWhitespace1) {
    auto lexer = sodium::Lexer("  \n  \t \r\f \v   ");
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::EOF_TOKEN, token.kind());
    EXPECT_EQ("", token.value());
}

TEST(LexerTest, LexerSkipsWhitespace2) {
    auto lexer = sodium::Lexer("   \n\n \t \ridentifier\f \v   ");
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token.kind());
    EXPECT_EQ("identifier", token.value());
}

TEST(LexerTest, LexerRejectsInvalidToken) {
    auto lexer = sodium::Lexer("$");
    auto token = lexer.next_token();

    EXPECT_TRUE(sodium::ErrorManager::has_errors());
}

TEST(LexerTest, LexerRejectsInvalidIdentifier1) {
    auto lexer = sodium::Lexer("$identifier");
    auto token = lexer.next_token();

    EXPECT_TRUE(sodium::ErrorManager::has_errors());
}

TEST(LexerTest, LexerRejectsInvalidIdentifier2) {
    auto lexer = sodium::Lexer("ident$ifier");
    auto token = lexer.next_token();

    EXPECT_TRUE(sodium::ErrorManager::has_errors());
}
