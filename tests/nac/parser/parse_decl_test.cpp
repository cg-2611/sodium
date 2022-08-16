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
        func name() -> int { return 0 }
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
    auto rightBrace = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_BRACE, "}", 1, 0, 0);

    returnValue->next(std::move(rightBrace));
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
    EXPECT_EQ(0, funcDecl->signature()->parameters()->parameters().size());
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
    auto signature = parser.parseSignature();

    EXPECT_EQ(sodium::ASTNodeKind::SIGNATURE, signature->nodeKind());
    EXPECT_EQ(functionNameString, signature->name()->value());
    EXPECT_EQ(0, signature->parameters()->parameters().size());
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

    EXPECT_EQ(sodium::ASTNodeKind::PARAMETER_LIST, parameterList->nodeKind());
    EXPECT_EQ(0, parameterList->parameters().size());
}

/*
    tests parameter list:
        (param)
*/
TEST(ParseDeclTest, ParserCorrectlyParsesAnParameterListWithASingleParameter) {
    std::string_view parameterString("param");

    auto leftParen = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_PAREN, ")", 1, 0, 0);
    auto parameter = std::make_unique<sodium::Token>(sodium::TokenKind::IDENTIFIER, parameterString.data(),
                                                     parameterString.size(), 0, 0);
    auto rightParen = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_PAREN, "(", 1, 0, 0);

    parameter->next(std::move(rightParen));
    leftParen->next(std::move(parameter));

    sodium::Parser parser(leftParen.get());
    auto parameterList = parser.parseParameterList();

    EXPECT_EQ(sodium::ASTNodeKind::PARAMETER_LIST, parameterList->nodeKind());

    ASSERT_EQ(1, parameterList->parameters().size());

    EXPECT_EQ(sodium::ASTNodeKind::PARAMETER, parameterList->parameters()[0]->nodeKind());
    EXPECT_EQ(parameterString, parameterList->parameters()[0]->identifier()->value());
}

TEST(ParseDeclTest, ParserCorrectlyParsesAnParameterListWithMultipleParameters) {
    std::string_view parameterString1("param1");
    std::string_view parameterString2("param2");

    auto leftParen = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_PAREN, ")", 1, 0, 0);
    auto parameter1 = std::make_unique<sodium::Token>(sodium::TokenKind::IDENTIFIER, parameterString1.data(),
                                                      parameterString1.size(), 0, 0);
    auto parameter2 = std::make_unique<sodium::Token>(sodium::TokenKind::IDENTIFIER, parameterString2.data(),
                                                      parameterString2.size(), 0, 0);
    auto rightParen = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_PAREN, "(", 1, 0, 0);

    parameter2->next(std::move(rightParen));
    parameter1->next(std::move(parameter2));
    leftParen->next(std::move(parameter1));

    sodium::Parser parser(leftParen.get());
    auto parameterList = parser.parseParameterList();

    EXPECT_EQ(sodium::ASTNodeKind::PARAMETER_LIST, parameterList->nodeKind());

    ASSERT_EQ(2, parameterList->parameters().size());

    EXPECT_EQ(sodium::ASTNodeKind::PARAMETER, parameterList->parameters()[0]->nodeKind());
    EXPECT_EQ(parameterString1, parameterList->parameters()[0]->identifier()->value());

    EXPECT_EQ(sodium::ASTNodeKind::PARAMETER, parameterList->parameters()[1]->nodeKind());
    EXPECT_EQ(parameterString2, parameterList->parameters()[1]->identifier()->value());
}

/*
    tests parameter:
        param
*/
TEST(ParseDeclTest, ParserCorrectlyParsesAParameter) {
    std::string_view parameterString("param");

    auto parameter = std::make_unique<sodium::Token>(sodium::TokenKind::IDENTIFIER, parameterString.data(),
                                                     parameterString.size(), 0, 0);

    sodium::Parser parser(parameter.get());
    auto parameterNode = parser.parseParameter();

    EXPECT_EQ(sodium::ASTNodeKind::PARAMETER, parameterNode->nodeKind());
    EXPECT_EQ(parameterString, parameterNode->identifier()->value());
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
