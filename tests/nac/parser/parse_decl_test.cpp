#include "sodium/nac/parser/parser.h"

#include <string_view>

#include "gtest/gtest.h"

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/decl.h"
#include "sodium/nac/ast/stmt.h"
#include "sodium/nac/diagnostics/diagnostic_engine.h"
#include "sodium/nac/lexer/lexer.h"

/*
    tests declaration:
        func name() -> int {}
*/
TEST(ParseDeclTest, ParserCorrectlyDispatchesToParseAFunctionDeclaration) {
    auto _ = sodium::DiagnosticEngine();

    auto src = std::string_view("func name() -> int {}");
    auto token_buffer = sodium::Lexer(src, _).tokenize();

    auto parser = sodium::Parser(token_buffer, _);
    auto decl = parser.parse_decl();

    EXPECT_EQ(sodium::ASTNodeKind::DECL, decl->node_kind());
    EXPECT_EQ(sodium::DeclKind::FUNC, decl->decl_kind());
}

/*
    tests function declaration:
        func name() -> int { return 0; }
*/
TEST(ParseDeclTest, ParserCorrectlyParsesAFunctionDeclaration) {
    auto _ = sodium::DiagnosticEngine();

    auto src = std::string_view("func name() -> int { return 0; }");
    auto token_buffer = sodium::Lexer(src, _).tokenize();

    auto parser = sodium::Parser(token_buffer, _);
    auto func_decl = parser.parse_func_decl();

    EXPECT_EQ(sodium::ASTNodeKind::DECL, func_decl->node_kind());
    EXPECT_EQ(sodium::DeclKind::FUNC, func_decl->decl_kind());
    EXPECT_EQ("name", func_decl->signature()->name()->value());
    EXPECT_EQ(sodium::ASTNodeKind::PARAMETER_LIST, func_decl->signature()->parameter_list()->node_kind());
    EXPECT_EQ("int", func_decl->signature()->return_type()->name());
    EXPECT_EQ(sodium::ASTNodeKind::STMT, func_decl->body()->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, func_decl->body()->stmt_kind());

    ASSERT_EQ(1, func_decl->body()->stmts().size());
    EXPECT_EQ(sodium::StmtKind::RETURN, func_decl->body()->stmts()[0]->stmt_kind());
}

/*
    tests function signature:
        func name() -> int
*/
TEST(ParseDeclTest, ParserCorrectlyParsesAFuncSignature) {
    auto _ = sodium::DiagnosticEngine();

    auto src = std::string_view("func name() -> int");
    auto token_buffer = sodium::Lexer(src, _).tokenize();

    auto parser = sodium::Parser(token_buffer, _);
    auto signature = parser.parse_func_signature();

    EXPECT_EQ(sodium::ASTNodeKind::FUNC_SIGNATURE, signature->node_kind());
    EXPECT_EQ("name", signature->name()->value());
    EXPECT_EQ(sodium::ASTNodeKind::PARAMETER_LIST, signature->parameter_list()->node_kind());
    EXPECT_EQ("int", signature->return_type()->name());
}

/*
    tests parameter list:
        ()
*/
TEST(ParseDeclTest, ParserCorrectlyParsesAnEmptyParameterList) {
    auto _ = sodium::DiagnosticEngine();

    auto src = std::string_view("()");
    auto token_buffer = sodium::Lexer(src, _).tokenize();

    auto parser = sodium::Parser(token_buffer, _);
    auto parameter_list = parser.parse_parameter_list();

    EXPECT_EQ(sodium::ASTNodeKind::PARAMETER_LIST, parameter_list->node_kind());
}

/*
    tests return type:
        -> int
*/
TEST(ParseDeclTest, ParserCorrectlyParsesAFunctionReturnType) {
    auto _ = sodium::DiagnosticEngine();

    auto src = std::string_view("-> int");
    auto token_buffer = sodium::Lexer(src, _).tokenize();

    auto parser = sodium::Parser(token_buffer, _);
    auto return_type_node = parser.parse_return_type();

    EXPECT_EQ(sodium::ASTNodeKind::TYPE, return_type_node->node_kind());
    EXPECT_EQ("int", return_type_node->name());
}
