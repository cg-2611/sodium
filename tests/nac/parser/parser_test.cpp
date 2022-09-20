#include "sodium/nac/parser/parser.h"

#include <string_view>

#include "gtest/gtest.h"

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/decl.h"
#include "sodium/nac/ast/identifier.h"
#include "sodium/nac/ast/source_file.h"
#include "sodium/nac/ast/type.h"
#include "sodium/nac/diagnostics/diagnostic.h"
#include "sodium/nac/diagnostics/diagnostic_engine.h"
#include "sodium/nac/lexer/lexer.h"
#include "sodium/nac/parser/parser_diagnostics.h"

/*
    tests source file:

*/
TEST(ParserTest, ParserCorrectlyParsesAnEmptySourceFile) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto source_file = parser.parse_source_file();

    EXPECT_EQ(sodium::ASTNodeKind::SOURCE_FILE, source_file->node_kind());
    EXPECT_EQ(0, source_file->decls().size());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

/*
    tests source file:
        func name() -> int {}
*/
TEST(ParserTest, ParserCorrectlyParsesASourceFileWithASingleDeclaration) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("func name() -> int {}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto source_file = parser.parse_source_file();

    EXPECT_EQ(sodium::ASTNodeKind::SOURCE_FILE, source_file->node_kind());

    ASSERT_EQ(1, source_file->decls().size());
    EXPECT_EQ(sodium::ASTNodeKind::DECL, source_file->decls()[0]->node_kind());
    EXPECT_EQ(sodium::DeclKind::FUNC, source_file->decls()[0]->decl_kind());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

/*
    tests source file:
        func name1() -> int {}
        func name2() -> int {}
*/
TEST(ParserTest, ParserCorrectlyParsesASourceFileWithMultipleDeclarations) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("func name1() -> int {}\n"
                                "func name2() -> int {}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto source_file = parser.parse_source_file();

    EXPECT_EQ(sodium::ASTNodeKind::SOURCE_FILE, source_file->node_kind());

    ASSERT_EQ(2, source_file->decls().size());
    EXPECT_EQ(sodium::ASTNodeKind::DECL, source_file->decls()[0]->node_kind());
    EXPECT_EQ(sodium::DeclKind::FUNC, source_file->decls()[0]->decl_kind());
    EXPECT_EQ(sodium::ASTNodeKind::DECL, source_file->decls()[1]->node_kind());
    EXPECT_EQ(sodium::DeclKind::FUNC, source_file->decls()[1]->decl_kind());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

/*
    tests source file:
        name() -> int {}
*/
TEST(ParserTest, ParserIgnoresInvalidDeclarationsInASourceFileAndDiagnosesParserError) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("name() -> int {}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto source_file = parser.parse_source_file();

    EXPECT_EQ(sodium::ASTNodeKind::SOURCE_FILE, source_file->node_kind());
    EXPECT_EQ(0, source_file->decls().size());

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
    tests source file:
        name1() -> int {}
        func name2() -> int {}
*/
TEST(ParserTest, ParserRecoversAfterDiagnosingParserErrorWhenParsingADeclaration) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("name1() -> int {}\n"
                                "func name2() -> int {}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto source_file = parser.parse_source_file();

    EXPECT_EQ(sodium::ASTNodeKind::SOURCE_FILE, source_file->node_kind());

    ASSERT_EQ(1, source_file->decls().size());
    EXPECT_EQ(sodium::ASTNodeKind::DECL, source_file->decls()[0]->node_kind());
    EXPECT_EQ(sodium::DeclKind::FUNC, source_file->decls()[0]->decl_kind());

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
    tests source file:
        name1() -> int {}
        func name2() -> int {}
        name3() -> int {}
*/
TEST(ParserTest, ParserDiagnosesMultipleParserErrorWhenParsingAMultipleInvalidDeclarations1) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("name1() -> int {}\n"
                                "func name2() -> int {}\n"
                                "name3() -> int {}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto source_file = parser.parse_source_file();

    EXPECT_EQ(sodium::ASTNodeKind::SOURCE_FILE, source_file->node_kind());

    ASSERT_EQ(1, source_file->decls().size());
    EXPECT_EQ(sodium::ASTNodeKind::DECL, source_file->decls()[0]->node_kind());
    EXPECT_EQ(sodium::DeclKind::FUNC, source_file->decls()[0]->decl_kind());

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(2, diagnostics.count_errors());

    ASSERT_EQ(2, diagnostics.count());

    auto *diagnostic1 = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic1);

    auto *parser_error1 = dynamic_cast<sodium::ParserError *>(diagnostic1);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error1->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_DECLARATION, parser_error1->kind());

    auto *diagnostic2 = diagnostics.get(1);
    ASSERT_NE(nullptr, diagnostic2);

    auto *parser_error2 = dynamic_cast<sodium::ParserError *>(diagnostic2);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error2->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_DECLARATION, parser_error2->kind());
}

/*
    tests source file:
        name() -> int {}
        func () -> int {}
*/
TEST(ParserTest, ParserDiagnosesMultipleParserErrorWhenParsingAMultipleInvalidDeclarations2) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("name() -> int {}\n"
                                "func () -> int {}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto source_file = parser.parse_source_file();

    EXPECT_EQ(sodium::ASTNodeKind::SOURCE_FILE, source_file->node_kind());

    EXPECT_EQ(0, source_file->decls().size());

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(2, diagnostics.count_errors());

    ASSERT_EQ(2, diagnostics.count());

    auto *diagnostic1 = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic1);

    auto *parser_error1 = dynamic_cast<sodium::ParserError *>(diagnostic1);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error1->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_DECLARATION, parser_error1->kind());

    auto *diagnostic2 = diagnostics.get(1);
    ASSERT_NE(nullptr, diagnostic2);

    auto *parser_error2 = dynamic_cast<sodium::ParserError *>(diagnostic2);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error2->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_IDENTIFIER, parser_error2->kind());
}

/*
    tests identifier:
        identifier
*/
TEST(ParserTest, ParserCorrectlyParsesAnIdentifier) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("identifier");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto identifier_node = parser.parse_identifier();

    EXPECT_EQ(sodium::ASTNodeKind::IDENTIFIER, identifier_node->node_kind());
    EXPECT_EQ(src, identifier_node->value());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

/*
    tests identifier:
        func
*/
TEST(ParserTest, ParserDiagnosesParserErrorWhenIdentifierExpectedButNotGiven) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("func");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto identifier = parser.parse_identifier();

    EXPECT_EQ(nullptr, identifier);

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
    tests type:
        int
*/
TEST(ParserTest, ParserCorrectlyParsesAType) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("int");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto type_node = parser.parse_type();

    EXPECT_EQ(sodium::ASTNodeKind::TYPE, type_node->node_kind());
    EXPECT_EQ(src, type_node->name());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

/*
    tests type:
        func
*/
TEST(ParserTest, ParserDiagnosesParserErrorWhenTypeExpectedButNotGiven) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("func");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto type = parser.parse_type();

    EXPECT_EQ(nullptr, type);

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_TYPE, parser_error->kind());
}
