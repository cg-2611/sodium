#include "sodium/nac/lexer/lexer.h"

#include <optional>
#include <string_view>

#include "gtest/gtest.h"

#include "sodium/nac/diagnostics/diagnostic_engine.h"
#include "sodium/nac/token/token.h"
#include "sodium/nac/token/token_buffer.h"

TEST(LexerTest, LexerReadsFuncKeyword) {
    auto _ = sodium::DiagnosticEngine();

    auto func_keyword_string = std::string_view("func");
    auto lexer = sodium::Lexer(func_keyword_string, _);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::KEYWORD_FUNC, token.kind());
    EXPECT_EQ(func_keyword_string, token.value());
}

TEST(LexerTest, LexerReadsReturnKeyword) {
    auto _ = sodium::DiagnosticEngine();

    auto return_keyword_string = std::string_view("return");
    auto lexer = sodium::Lexer(return_keyword_string, _);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::KEYWORD_RETURN, token.kind());
    EXPECT_EQ(return_keyword_string, token.value());
}

TEST(LexerTest, LexerReadsIntType) {
    auto _ = sodium::DiagnosticEngine();

    auto int_type_string = std::string_view("int");
    auto lexer = sodium::Lexer(int_type_string, _);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::TYPE, token.kind());
    EXPECT_EQ(int_type_string, token.value());
}

TEST(LexerTest, LexerReadsValidIdentifier) {
    auto _ = sodium::DiagnosticEngine();

    auto identifier_string = std::string_view("identifier");
    auto lexer = sodium::Lexer(identifier_string, _);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token.kind());
    EXPECT_EQ(identifier_string, token.value());
}

TEST(LexerTest, LexerReadsValidIdentifierWithUnderscorePrefix) {
    auto _ = sodium::DiagnosticEngine();

    auto identifier_string = std::string_view("_identifier");
    auto lexer = sodium::Lexer(identifier_string, _);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token.kind());
    EXPECT_EQ(identifier_string, token.value());
}

TEST(LexerTest, LexerReadsIntegerLiteral) {
    auto _ = sodium::DiagnosticEngine();

    auto integer_literal_string = std::string_view("2");
    auto lexer = sodium::Lexer(integer_literal_string, _);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::INTEGER_LITERAL, token.kind());
    EXPECT_EQ(integer_literal_string, token.value());
}

TEST(LexerTest, LexerReadsIntegerLiteralWithMultipleDigits) {
    auto _ = sodium::DiagnosticEngine();

    auto integer_literal_string = std::string_view("123456789");
    auto lexer = sodium::Lexer(integer_literal_string, _);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::INTEGER_LITERAL, token.kind());
    EXPECT_EQ(integer_literal_string, token.value());
}

TEST(LexerTest, LexerReadsLeftBrace) {
    auto _ = sodium::DiagnosticEngine();

    auto left_brace_string = std::string_view("{");
    auto lexer = sodium::Lexer(left_brace_string, _);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::LEFT_BRACE, token.kind());
    EXPECT_EQ(left_brace_string, token.value());
}

TEST(LexerTest, LexerReadsRightBrace) {
    auto _ = sodium::DiagnosticEngine();

    auto right_brace_string = std::string_view("}");
    auto lexer = sodium::Lexer(right_brace_string, _);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::RIGHT_BRACE, token.kind());
    EXPECT_EQ(right_brace_string, token.value());
}

TEST(LexerTest, LexerReadsLeftParen) {
    auto _ = sodium::DiagnosticEngine();

    auto left_paren_string = std::string_view("(");
    auto lexer = sodium::Lexer(left_paren_string, _);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::LEFT_PAREN, token.kind());
    EXPECT_EQ(left_paren_string, token.value());
}

TEST(LexerTest, LexerReadsRightParen) {
    auto _ = sodium::DiagnosticEngine();

    auto right_paren_string = std::string_view(")");
    auto lexer = sodium::Lexer(right_paren_string, _);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::RIGHT_PAREN, token.kind());
    EXPECT_EQ(right_paren_string, token.value());
}

TEST(LexerTest, LexerReadsSemicolon) {
    auto _ = sodium::DiagnosticEngine();

    auto semicolon_string = std::string_view(";");
    auto lexer = sodium::Lexer(semicolon_string, _);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::SEMICOLON, token.kind());
    EXPECT_EQ(semicolon_string, token.value());
}

TEST(LexerTest, LexerReadsArrow) {
    auto _ = sodium::DiagnosticEngine();

    auto arrow_string = std::string_view("->");
    auto lexer = sodium::Lexer(arrow_string, _);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::ARROW, token.kind());
    EXPECT_EQ(arrow_string, token.value());
}

TEST(LexerTest, LexerReadsEmpty_string) {
    auto _ = sodium::DiagnosticEngine();

    auto empty_string = std::string_view("");
    auto lexer = sodium::Lexer(empty_string, _);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::EOF_TOKEN, token.kind());
    EXPECT_EQ(empty_string, token.value());
}

TEST(LexerTest, LexerSkipsWhitespace1) {
    auto _ = sodium::DiagnosticEngine();

    auto lexer = sodium::Lexer("  \n  \t \r\f \v   ", _);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::EOF_TOKEN, token.kind());
    EXPECT_EQ("", token.value());
}

TEST(LexerTest, LexerSkipsWhitespace2) {
    auto _ = sodium::DiagnosticEngine();

    auto lexer = sodium::Lexer("   \n\n \t \ridentifier\f \v   ", _);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token.kind());
    EXPECT_EQ("identifier", token.value());
}

TEST(LexerTest, LexerReadsMultipleTokens) {
    auto _ = sodium::DiagnosticEngine();

    auto src = std::string_view("identifier 2 func; { -> ) int");
    auto lexer = sodium::Lexer(src, _);
    auto token_buffer = lexer.tokenize();

    ASSERT_EQ(9, token_buffer.size()); // 9 because of eof token

    for (int i = 0; i < token_buffer.size(); ++i) {
        ASSERT_TRUE(token_buffer.get(i).has_value());
    }

    auto token1 = token_buffer.get(0).value();
    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token1.kind());
    EXPECT_EQ("identifier", token1.value());

    auto token2 = token_buffer.get(1).value();
    EXPECT_EQ(sodium::TokenKind::INTEGER_LITERAL, token2.kind());
    EXPECT_EQ("2", token2.value());

    auto token3 = token_buffer.get(2).value();
    EXPECT_EQ(sodium::TokenKind::KEYWORD_FUNC, token3.kind());
    EXPECT_EQ("func", token3.value());

    auto token4 = token_buffer.get(3).value();
    EXPECT_EQ(sodium::TokenKind::SEMICOLON, token4.kind());
    EXPECT_EQ(";", token4.value());

    auto token5 = token_buffer.get(4).value();
    EXPECT_EQ(sodium::TokenKind::LEFT_BRACE, token5.kind());
    EXPECT_EQ("{", token5.value());

    auto token6 = token_buffer.get(5).value();
    EXPECT_EQ(sodium::TokenKind::ARROW, token6.kind());
    EXPECT_EQ("->", token6.value());

    auto token7 = token_buffer.get(6).value();
    EXPECT_EQ(sodium::TokenKind::RIGHT_PAREN, token7.kind());
    EXPECT_EQ(")", token7.value());

    auto token8 = token_buffer.get(7).value();
    EXPECT_EQ(sodium::TokenKind::TYPE, token8.kind());
    EXPECT_EQ("int", token8.value());

    auto token9 = token_buffer.get(8).value();
    EXPECT_EQ(sodium::TokenKind::EOF_TOKEN, token9.kind());
    EXPECT_EQ("", token9.value());
}

TEST(LexerTest, LexerRejectsInvalidToken) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto lexer = sodium::Lexer("$", diagnostics);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::ERROR_TOKEN, token.kind());
    EXPECT_EQ("$", token.value());

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count());
    EXPECT_EQ(1, diagnostics.count_errors());
}

TEST(LexerTest, LexerRejectsInvalidIdentifier1) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto lexer = sodium::Lexer("$identifier", diagnostics);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::ERROR_TOKEN, token.kind());
    EXPECT_EQ("$", token.value());

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count());
    EXPECT_EQ(1, diagnostics.count_errors());
}

TEST(LexerTest, LexerRejectsInvalidIdentifier2) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto lexer = sodium::Lexer("ident$ifier", diagnostics);

    auto _ = lexer.next_token();
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::ERROR_TOKEN, token.kind());
    EXPECT_EQ("$", token.value());

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count());
    EXPECT_EQ(1, diagnostics.count_errors());
}
