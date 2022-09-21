#include "sodium/nac/diagnostics/diagnostic.h"
#include "sodium/nac/lexer/lexer.h"

#include <optional>
#include <string_view>

#include "gtest/gtest.h"

#include "sodium/nac/diagnostics/diagnostic_engine.h"
#include "sodium/nac/lexer/lexer_diagnostics.h"
#include "sodium/nac/token/token.h"
#include "sodium/nac/token/token_buffer.h"

TEST(LexerTest, LexerReadsFuncKeyword) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto func_keyword_string = std::string_view("func");
    auto lexer = sodium::Lexer(func_keyword_string, diagnostics);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::KEYWORD_FUNC, token.kind());
    EXPECT_EQ(func_keyword_string, token.value());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

TEST(LexerTest, LexerReadsReturnKeyword) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto return_keyword_string = std::string_view("return");
    auto lexer = sodium::Lexer(return_keyword_string, diagnostics);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::KEYWORD_RETURN, token.kind());
    EXPECT_EQ(return_keyword_string, token.value());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

TEST(LexerTest, LexerReadsIntType) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto int_type_string = std::string_view("int");
    auto lexer = sodium::Lexer(int_type_string, diagnostics);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::TYPE, token.kind());
    EXPECT_EQ(int_type_string, token.value());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

TEST(LexerTest, LexerReadsValidIdentifier) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto identifier_string = std::string_view("identifier");
    auto lexer = sodium::Lexer(identifier_string, diagnostics);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token.kind());
    EXPECT_EQ(identifier_string, token.value());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

TEST(LexerTest, LexerReadsValidIdentifierWithUnderscorePrefix) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto identifier_string = std::string_view("_identifier");
    auto lexer = sodium::Lexer(identifier_string, diagnostics);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token.kind());
    EXPECT_EQ(identifier_string, token.value());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

TEST(LexerTest, LexerReadsIntegerLiteral) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto integer_literal_string = std::string_view("2");
    auto lexer = sodium::Lexer(integer_literal_string, diagnostics);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::INTEGER_LITERAL, token.kind());
    EXPECT_EQ(integer_literal_string, token.value());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

TEST(LexerTest, LexerReadsIntegerLiteralWithMultipleDigits) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto integer_literal_string = std::string_view("123456789");
    auto lexer = sodium::Lexer(integer_literal_string, diagnostics);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::INTEGER_LITERAL, token.kind());
    EXPECT_EQ(integer_literal_string, token.value());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

TEST(LexerTest, LexerReadsLeftBrace) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto left_brace_string = std::string_view("{");
    auto lexer = sodium::Lexer(left_brace_string, diagnostics);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::LEFT_BRACE, token.kind());
    EXPECT_EQ(left_brace_string, token.value());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

TEST(LexerTest, LexerReadsRightBrace) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto right_brace_string = std::string_view("}");
    auto lexer = sodium::Lexer(right_brace_string, diagnostics);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::RIGHT_BRACE, token.kind());
    EXPECT_EQ(right_brace_string, token.value());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

TEST(LexerTest, LexerReadsLeftParen) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto left_paren_string = std::string_view("(");
    auto lexer = sodium::Lexer(left_paren_string, diagnostics);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::LEFT_PAREN, token.kind());
    EXPECT_EQ(left_paren_string, token.value());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

TEST(LexerTest, LexerReadsRightParen) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto right_paren_string = std::string_view(")");
    auto lexer = sodium::Lexer(right_paren_string, diagnostics);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::RIGHT_PAREN, token.kind());
    EXPECT_EQ(right_paren_string, token.value());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

TEST(LexerTest, LexerReadsSemicolon) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto semicolon_string = std::string_view(";");
    auto lexer = sodium::Lexer(semicolon_string, diagnostics);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::SEMICOLON, token.kind());
    EXPECT_EQ(semicolon_string, token.value());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

TEST(LexerTest, LexerReadsArrow) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto arrow_string = std::string_view("->");
    auto lexer = sodium::Lexer(arrow_string, diagnostics);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::ARROW, token.kind());
    EXPECT_EQ(arrow_string, token.value());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

TEST(LexerTest, LexerReadsEmpty_string) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto empty_string = std::string_view("");
    auto lexer = sodium::Lexer(empty_string, diagnostics);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::EOF_TOKEN, token.kind());
    EXPECT_EQ(empty_string, token.value());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

TEST(LexerTest, LexerSkipsWhitespace1) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto lexer = sodium::Lexer("  \n  \t \r\f \v   ", diagnostics);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::EOF_TOKEN, token.kind());
    EXPECT_EQ("", token.value());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

TEST(LexerTest, LexerSkipsWhitespace2) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto lexer = sodium::Lexer("   \n\n \t \ridentifier\f \v   ", diagnostics);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::IDENTIFIER, token.kind());
    EXPECT_EQ("identifier", token.value());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

TEST(LexerTest, LexerReadsMultipleTokens) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("identifier 2 func; { -> ) int");
    auto lexer = sodium::Lexer(src, diagnostics);
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

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

TEST(LexerTest, LexerRejectsInvalidTokenAndDiagnosesLexerError) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto lexer = sodium::Lexer("$", diagnostics);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::ERROR_TOKEN, token.kind());
    EXPECT_EQ("$", token.value());

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *lexer_error = dynamic_cast<sodium::LexerError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, lexer_error->diagnostic_kind());
    EXPECT_EQ(sodium::LexerErrorKind::UNRECOGNISED_TOKEN, lexer_error->kind());
}

TEST(LexerTest, LexerRejectsInvalidIdentifierAndDiagnosesLexerError1) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto lexer = sodium::Lexer("$identifier", diagnostics);
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::ERROR_TOKEN, token.kind());
    EXPECT_EQ("$", token.value());

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *lexer_error = dynamic_cast<sodium::LexerError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, lexer_error->diagnostic_kind());
    EXPECT_EQ(sodium::LexerErrorKind::UNRECOGNISED_TOKEN, lexer_error->kind());
}

TEST(LexerTest, LexerRejectsInvalidIdentifierAndDiagnosesLexerError2) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto lexer = sodium::Lexer("ident$ifier", diagnostics);

    auto _ = lexer.next_token(); // ignore first token, only interested in error token
    auto token = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::ERROR_TOKEN, token.kind());
    EXPECT_EQ("$", token.value());

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *lexer_error = dynamic_cast<sodium::LexerError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, lexer_error->diagnostic_kind());
    EXPECT_EQ(sodium::LexerErrorKind::UNRECOGNISED_TOKEN, lexer_error->kind());
}

TEST(LexerTest, LexerDiagnosesMultipleLexerErrorsForMultipleInvalidTokens) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto lexer = sodium::Lexer("ide$ntif$ier", diagnostics);

    auto _ = lexer.next_token(); // ignore first token, only interested in error tokens
    auto error_token1 = lexer.next_token();
    _ = lexer.next_token(); // ignore third token, only interested in error tokens
    auto error_token2 = lexer.next_token();

    EXPECT_EQ(sodium::TokenKind::ERROR_TOKEN, error_token1.kind());
    EXPECT_EQ("$", error_token1.value());

    EXPECT_EQ(sodium::TokenKind::ERROR_TOKEN, error_token2.kind());
    EXPECT_EQ("$", error_token2.value());

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(2, diagnostics.count_errors());

    ASSERT_EQ(2, diagnostics.count());

    auto *diagnostic1 = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic1);

    auto *lexer_error1 = dynamic_cast<sodium::LexerError *>(diagnostic1);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, lexer_error1->diagnostic_kind());
    EXPECT_EQ(sodium::LexerErrorKind::UNRECOGNISED_TOKEN, lexer_error1->kind());

    auto *diagnostic2 = diagnostics.get(1);
    ASSERT_NE(nullptr, diagnostic2);

    auto *lexer_error2 = dynamic_cast<sodium::LexerError *>(diagnostic2);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, lexer_error2->diagnostic_kind());
    EXPECT_EQ(sodium::LexerErrorKind::UNRECOGNISED_TOKEN, lexer_error2->kind());
}
