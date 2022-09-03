#include "sodium/nac/parser/parser.h"

#include <memory>
#include <string_view>
#include <utility>

#include <gtest/gtest.h>

#include "sodium/nac/ast/decl.h"
#include "sodium/nac/ast/stmt.h"
#include "sodium/nac/lexer/token.h"

/*
    tests declaration:
        func name() -> int {}
*/
TEST(ParseDeclTest, ParserCorrectlyDispatchesToParseAFunctionDeclaration) {
    std::string_view funcKeywordString("func");
    std::string_view functionNameString("name");
    std::string_view returnTypeString("int");

    auto funcKeyword = std::make_unique<sodium::Token>(sodium::TokenKind::KEYWORD, funcKeywordString.data(),
                                                       funcKeywordString.size(), 0, 0);
    auto functionName = std::make_unique<sodium::Token>(sodium::TokenKind::IDENTIFIER, functionNameString.data(),
                                                        functionNameString.size(), 0, 0);
    auto leftParen = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_PAREN, ")", 1, 0, 0);
    auto rightParen = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_PAREN, "(", 1, 0, 0);
    auto arrow = std::make_unique<sodium::Token>(sodium::TokenKind::ARROW, "->", 2, 0, 0);
    auto returnType = std::make_unique<sodium::Token>(sodium::TokenKind::TYPE, returnTypeString.data(),
                                                      returnTypeString.size(), 0, 0);
    auto leftBrace = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_BRACE, "}", 1, 0, 0);
    auto rightBrace = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_BRACE, "{", 1, 0, 0);

    leftBrace->next(std::move(rightBrace));
    returnType->next(std::move(leftBrace));
    arrow->next(std::move(returnType));
    rightParen->next(std::move(arrow));
    leftParen->next(std::move(rightParen));
    functionName->next(std::move(leftParen));
    funcKeyword->next(std::move(functionName));

    sodium::Parser parser(funcKeyword.get());
    auto decl = parser.parseDecl();

    EXPECT_EQ(sodium::ASTNodeKind::DECL, decl->nodeKind());
    EXPECT_EQ(sodium::DeclKind::FUNCTION, decl->declKind());
}

/*
    tests function declaration:
        func name() -> int { return 0; }
*/
TEST(ParseDeclTest, ParserCorrectlyParsesAFunctionDeclaration) {
    std::string_view funcKeywordString("func");
    std::string_view functionNameString("name");
    std::string_view returnTypeString("int");
    std::string_view returnKeywordString("return");
    std::string_view returnValueString("0");

    auto funcKeyword = std::make_unique<sodium::Token>(sodium::TokenKind::KEYWORD, funcKeywordString.data(),
                                                       funcKeywordString.size(), 0, 0);
    auto functionName = std::make_unique<sodium::Token>(sodium::TokenKind::IDENTIFIER, functionNameString.data(),
                                                        functionNameString.size(), 0, 0);
    auto leftParen = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_PAREN, ")", 1, 0, 0);
    auto rightParen = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_PAREN, "(", 1, 0, 0);
    auto arrow = std::make_unique<sodium::Token>(sodium::TokenKind::ARROW, "->", 2, 0, 0);
    auto returnType = std::make_unique<sodium::Token>(sodium::TokenKind::TYPE, returnTypeString.data(),
                                                      returnTypeString.size(), 0, 0);
    auto leftBrace = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_BRACE, "{", 1, 0, 0);
    auto returnKeyword = std::make_unique<sodium::Token>(sodium::TokenKind::KEYWORD, returnKeywordString.data(),
                                                         returnKeywordString.size(), 0, 0);
    auto returnValue = std::make_unique<sodium::Token>(sodium::TokenKind::NUMERIC_LITERAL, returnValueString.data(),
                                                       returnValueString.size(), 0, 0);
    auto semicolon = std::make_unique<sodium::Token>(sodium::TokenKind::SEMICOLON, ";", 1, 0, 0);
    auto rightBrace = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_BRACE, "}", 1, 0, 0);

    semicolon->next(std::move(rightBrace));
    returnValue->next(std::move(semicolon));
    returnKeyword->next(std::move(returnValue));
    leftBrace->next(std::move(returnKeyword));
    returnType->next(std::move(leftBrace));
    arrow->next(std::move(returnType));
    rightParen->next(std::move(arrow));
    leftParen->next(std::move(rightParen));
    functionName->next(std::move(leftParen));
    funcKeyword->next(std::move(functionName));

    sodium::Parser parser(funcKeyword.get());
    auto funcDecl = parser.parseFuncDecl();

    EXPECT_EQ(sodium::ASTNodeKind::DECL, funcDecl->nodeKind());
    EXPECT_EQ(sodium::DeclKind::FUNCTION, funcDecl->declKind());
    EXPECT_EQ(functionNameString, funcDecl->signature()->name()->value());
    EXPECT_EQ(sodium::DeclKind::PARAMETER_LIST, funcDecl->signature()->parameterList()->declKind());
    EXPECT_EQ(returnTypeString, funcDecl->signature()->returnType()->name());
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
    std::string_view funcKeywordString("func");
    std::string_view functionNameString("name");
    std::string_view returnTypeString("int");

    auto funcKeyword = std::make_unique<sodium::Token>(sodium::TokenKind::KEYWORD, funcKeywordString.data(),
                                                       funcKeywordString.size(), 0, 0);
    auto functionName = std::make_unique<sodium::Token>(sodium::TokenKind::IDENTIFIER, functionNameString.data(),
                                                        functionNameString.size(), 0, 0);
    auto leftParen = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_PAREN, ")", 1, 0, 0);
    auto rightParen = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_PAREN, "(", 1, 0, 0);
    auto arrow = std::make_unique<sodium::Token>(sodium::TokenKind::ARROW, "->", 2, 0, 0);
    auto returnType = std::make_unique<sodium::Token>(sodium::TokenKind::TYPE, returnTypeString.data(),
                                                      returnTypeString.size(), 0, 0);

    arrow->next(std::move(returnType));
    rightParen->next(std::move(arrow));
    leftParen->next(std::move(rightParen));
    functionName->next(std::move(leftParen));
    funcKeyword->next(std::move(functionName));

    sodium::Parser parser(funcKeyword.get());
    auto signature = parser.parseFunctionSignature();

    EXPECT_EQ(sodium::DeclKind::FUNCTION_SIGNATURE, signature->declKind());
    EXPECT_EQ(functionNameString, signature->name()->value());
    EXPECT_EQ(sodium::DeclKind::PARAMETER_LIST, signature->parameterList()->declKind());
    EXPECT_EQ(returnTypeString, signature->returnType()->name());
}

/*
    tests parameter list:
        ()
*/
TEST(ParseDeclTest, ParserCorrectlyParsesAnEmptyParameterList) {
    auto leftParen = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_PAREN, ")", 1, 0, 0);
    auto rightParen = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_PAREN, "(", 1, 0, 0);

    leftParen->next(std::move(rightParen));

    sodium::Parser parser(leftParen.get());
    auto parameterList = parser.parseParameterList();

    EXPECT_EQ(sodium::ASTNodeKind::DECL, parameterList->nodeKind());
    EXPECT_EQ(sodium::DeclKind::PARAMETER_LIST, parameterList->declKind());
}

/*
    tests return type:
        -> int
*/
TEST(ParseDeclTest, ParserCorrectlyParsesAFunctionReturnType) {
    std::string_view returnTypeString("int");

    auto arrow = std::make_unique<sodium::Token>(sodium::TokenKind::ARROW, "->", 2, 0, 0);
    auto returnType = std::make_unique<sodium::Token>(sodium::TokenKind::TYPE, returnTypeString.data(),
                                                      returnTypeString.size(), 0, 0);

    arrow->next(std::move(returnType));

    sodium::Parser parser(arrow.get());
    auto returnTypeNode = parser.parseReturnType();

    EXPECT_EQ(sodium::ASTNodeKind::TYPE, returnTypeNode->nodeKind());
    EXPECT_EQ(returnTypeString, returnTypeNode->name());
}
