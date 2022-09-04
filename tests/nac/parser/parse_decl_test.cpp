#include "sodium/nac/parser/parser.h"

#include <memory>
#include <string_view>
#include <utility>

#include <gtest/gtest.h>

#include "sodium/nac/ast/decl.h"
#include "sodium/nac/ast/stmt.h"
#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/lexer/token.h"

/*
    tests declaration:
        func name() -> int {}
*/
TEST(ParseDeclTest, ParserCorrectlyDispatchesToParseAFunctionDeclaration) {
    std::string_view src("func name() -> int {}");

    sodium::Parser parser(src);
    auto decl = parser.parseDecl();

    EXPECT_EQ(sodium::ASTNodeKind::DECL, decl->nodeKind());
    EXPECT_EQ(sodium::DeclKind::FUNCTION, decl->declKind());
}

/*
    tests function declaration:
        func name() -> int { return 0; }
*/
TEST(ParseDeclTest, ParserCorrectlyParsesAFunctionDeclaration) {
    std::string_view src("func name() -> int { return 0; }");

    sodium::Parser parser(src);
    auto funcDecl = parser.parseFuncDecl();

    EXPECT_EQ(sodium::ASTNodeKind::DECL, funcDecl->nodeKind());
    EXPECT_EQ(sodium::DeclKind::FUNCTION, funcDecl->declKind());
    EXPECT_EQ("name", funcDecl->signature()->name()->value());
    EXPECT_EQ(sodium::DeclKind::PARAMETER_LIST, funcDecl->signature()->parameterList()->declKind());
    EXPECT_EQ("int", funcDecl->signature()->returnType()->name());
    EXPECT_EQ(sodium::ASTNodeKind::STMT, funcDecl->body()->nodeKind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, funcDecl->body()->stmtKind());

    ASSERT_EQ(1, funcDecl->body()->stmts().size());
    EXPECT_EQ(sodium::StmtKind::RETURN, funcDecl->body()->stmts()[0]->stmtKind());
}

/*
    tests function signature:
        func name() -> int
*/
TEST(ParseDeclTest, ParserCorrectlyParsesAFunctionSignature) {
    std::string_view src("func name() -> int {}");

    sodium::Parser parser(src);
    auto signature = parser.parseFunctionSignature();

    EXPECT_EQ(sodium::DeclKind::FUNCTION_SIGNATURE, signature->declKind());
    EXPECT_EQ("name", signature->name()->value());
    EXPECT_EQ(sodium::DeclKind::PARAMETER_LIST, signature->parameterList()->declKind());
    EXPECT_EQ("int", signature->returnType()->name());
}

/*
    tests parameter list:
        ()
*/
TEST(ParseDeclTest, ParserCorrectlyParsesAnEmptyParameterList) {
    sodium::Parser parser("()");
    auto parameterList = parser.parseParameterList();

    EXPECT_EQ(sodium::ASTNodeKind::DECL, parameterList->nodeKind());
    EXPECT_EQ(sodium::DeclKind::PARAMETER_LIST, parameterList->declKind());
}

/*
    tests return type:
        -> int
*/
TEST(ParseDeclTest, ParserCorrectlyParsesAFunctionReturnType) {
    sodium::Parser parser("-> int");
    auto returnTypeNode = parser.parseReturnType();

    EXPECT_EQ(sodium::ASTNodeKind::TYPE, returnTypeNode->nodeKind());
    EXPECT_EQ("int", returnTypeNode->name());
}
