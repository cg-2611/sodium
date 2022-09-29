#include "sodium/nac/parser/parser.h"

#include <string_view>

#include "gtest/gtest.h"

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/decl.h"
#include "sodium/nac/ast/stmt.h"
#include "sodium/nac/diagnostics/diagnostic_engine.h"
#include "sodium/nac/lexer/lexer.h"
#include "sodium/nac/parser/parser_diagnostics.h"

/*
    tests declaration:
        func name() -> int {}
*/
TEST(ParseDeclTest, ParserCorrectlyDispatchesToParseAFunctionDeclaration) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("func name() -> int {}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto decl = parser.parse_decl();

    EXPECT_EQ(sodium::ASTNodeKind::DECL, decl->node_kind());
    EXPECT_EQ(sodium::DeclKind::FUNC, decl->decl_kind());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

/*
    tests declaration:
        name() -> int {}
*/
TEST(ParseDeclTest, ParserDiagnosesParserErrorForInvalidDeclaration) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("name() -> int {}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto decl = parser.parse_decl();

    EXPECT_EQ(nullptr, decl);

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_DECLARATION, parser_error->kind());
}

/*
    tests function declaration:
        func name() -> int {}
*/
TEST(ParseDeclTest, ParserCorrectlyParsesAFunctionDeclaration) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("func name() -> int {}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto func_decl = parser.parse_func_decl();

    EXPECT_EQ(sodium::ASTNodeKind::DECL, func_decl->node_kind());
    EXPECT_EQ(sodium::DeclKind::FUNC, func_decl->decl_kind());
    EXPECT_EQ("name", func_decl->name()->value());
    EXPECT_EQ("int", func_decl->return_type()->name());
    EXPECT_EQ(sodium::ASTNodeKind::STMT, func_decl->body()->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, func_decl->body()->stmt_kind());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

/*
    tests function declaration:
        name() -> int {}
*/
TEST(ParseDeclTest, ParserDiagnosesParserErrorForMissingFuncKeywordInFunctionDeclaration) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("name() -> int {}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto func_decl = parser.parse_func_decl();

    EXPECT_EQ(nullptr, func_decl);

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_KEYWORD_FUNC, parser_error->kind());
}

/*
    tests function declaration:
        func () -> int {}
*/
TEST(ParseDeclTest, ParserDiagnosesParserErrorForMissingIdentifierInFunctionDeclaration) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("func () -> int {}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto func_decl = parser.parse_func_decl();

    EXPECT_EQ(nullptr, func_decl);

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_IDENTIFIER, parser_error->kind());
}

/*
    tests function declaration:
        func name) -> int {}
*/
TEST(ParseDeclTest, ParserDiagnosesParserErrorForMissingLeftParenForParameterListInFunctionDeclaration) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("func name) -> int {}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto func_decl = parser.parse_func_decl();

    EXPECT_EQ(nullptr, func_decl);

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_LEFT_PAREN_PARAMETER_LIST, parser_error->kind());
}

/*
    tests function declaration:
        func name( -> int {}
*/
TEST(ParseDeclTest, ParserDiagnosesParserErrorForMissingRightParenForParameterListInFunctionDeclaration) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("func name( -> int {}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto func_decl = parser.parse_func_decl();

    EXPECT_EQ(nullptr, func_decl);

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_RIGHT_PAREN_PARAMETER_LIST, parser_error->kind());
}

/*
    tests function declaration:
        func name() int {}
*/
TEST(ParseDeclTest, ParserDiagnosesParserErrorForMissingArrowInReturnTypeInFunctionDeclaration) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("func name() int {}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto func_decl = parser.parse_func_decl();

    EXPECT_EQ(nullptr, func_decl);

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_ARROW, parser_error->kind());
}

/*
    tests function declaration:
        func name() -> {}
*/
TEST(ParseDeclTest, ParserDiagnosesParserErrorForMissingTypeInReturnTypeInFunctionDeclaration) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("func name() -> {}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto func_decl = parser.parse_func_decl();

    EXPECT_EQ(nullptr, func_decl);

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_TYPE, parser_error->kind());
}

/*
    tests function declaration:
        func name() -> int
*/
TEST(ParseDeclTest, ParserDiagnosesParserErrorForFunctionDeclarationWithInvalidBody) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("func name() -> int");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto func_decl = parser.parse_func_decl();

    EXPECT_EQ(nullptr, func_decl);

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_LEFT_BRACE_BLOCK, parser_error->kind());
}

/*
    tests return type:
        -> int
*/
TEST(ParseDeclTest, ParserCorrectlyParsesAFunctionReturnType) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("-> int");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto return_type = parser.parse_return_type();

    EXPECT_EQ(sodium::ASTNodeKind::TYPE, return_type->node_kind());
    EXPECT_EQ("int", return_type->name());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

/*
    tests return type:
        int
*/
TEST(ParseDeclTest, ParserDiagnosesParserErrorForMissingArrowInReturnType) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("int");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto return_type = parser.parse_return_type();

    EXPECT_EQ(nullptr, return_type);

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_ARROW, parser_error->kind());
}

/*
    tests return type:
        ->
*/
TEST(ParseDeclTest, ParserDiagnosesParserErrorForMissingTypenReturnType) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("->");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto return_type = parser.parse_return_type();

    EXPECT_EQ(nullptr, return_type);

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_TYPE, parser_error->kind());
}
