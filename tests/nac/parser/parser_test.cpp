#include "sodium/nac/parser/parser.h"

#include <memory>
#include <string_view>
#include <utility>

#include <gtest/gtest.h>

#include "sodium/nac/ast/source_file.h"
#include "sodium/nac/ast/identifier.h"
#include "sodium/nac/ast/type.h"
#include "sodium/nac/lexer/token.h"

/*
    tests source file:

*/
TEST(ParserTest, ParserCorrectlyParsesAnEmptySourceFile) {
    auto eof = std::make_unique<sodium::Token>(sodium::TokenKind::EOF_TOKEN, "", 0, 0, 0);

    sodium::Parser parser(eof.get());
    auto sourceFile = parser.parseSourceFile();

    EXPECT_EQ(sodium::ASTNodeKind::SOURCE_FILE, sourceFile->nodeKind());
    EXPECT_EQ(0, sourceFile->decls().size());
}

/*
    tests source file:
        func name() -> int {}
*/
TEST(ParserTest, ParserCorrectlyParsesASourceFileWithASingleDeclaration) {
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
    auto leftBrace = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_BRACE, "{", 1, 0, 0);
    auto rightBrace = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_BRACE, "}", 1, 0, 0);
    auto eof = std::make_unique<sodium::Token>(sodium::TokenKind::EOF_TOKEN, "", 0, 0, 0);

    rightBrace->next(std::move(eof));
    leftBrace->next(std::move(rightBrace));
    returnType->next(std::move(leftBrace));
    arrow->next(std::move(returnType));
    rightParen->next(std::move(arrow));
    leftParen->next(std::move(rightParen));
    functionName->next(std::move(leftParen));
    funcKeyword->next(std::move(functionName));

    sodium::Parser parser(funcKeyword.get());
    auto sourceFile = parser.parseSourceFile();

    EXPECT_EQ(sodium::ASTNodeKind::SOURCE_FILE, sourceFile->nodeKind());

    ASSERT_EQ(1, sourceFile->decls().size());
    EXPECT_EQ(sodium::ASTNodeKind::DECL, sourceFile->decls()[0]->nodeKind());
    EXPECT_EQ(sodium::DeclKind::FUNCTION, sourceFile->decls()[0]->declKind());
}

/*
    tests source file:
        func name1() -> int {}
        func name2() -> int {}
*/
TEST(ParserTest, ParserCorrectlyParsesASourceFileWithMultipleDeclarationsOnDifferentLines) {
    std::string_view funcKeywordString1("func");
    std::string_view functionNameString1("name1");
    std::string_view returnTypeString1("int");
    std::string_view funcKeywordString2("func");
    std::string_view functionNameString2("name2");
    std::string_view returnTypeString2("int");

    auto funcKeyword1 = std::make_unique<sodium::Token>(sodium::TokenKind::KEYWORD, funcKeywordString1.data(),
                                                        funcKeywordString1.size(), 0, 0);
    auto functionName1 = std::make_unique<sodium::Token>(sodium::TokenKind::IDENTIFIER, functionNameString1.data(),
                                                         functionNameString1.size(), 0, 0);
    auto leftParen1 = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_PAREN, ")", 1, 0, 0);
    auto rightParen1 = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_PAREN, "(", 1, 0, 0);
    auto arrow1 = std::make_unique<sodium::Token>(sodium::TokenKind::ARROW, "->", 2, 0, 0);
    auto returnType1 = std::make_unique<sodium::Token>(sodium::TokenKind::TYPE, returnTypeString1.data(),
                                                       returnTypeString1.size(), 0, 0);
    auto leftBrace1 = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_BRACE, "{", 1, 0, 0);
    auto rightBrace1 = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_BRACE, "}", 1, 0, 0);
    auto funcKeyword2 = std::make_unique<sodium::Token>(sodium::TokenKind::KEYWORD, funcKeywordString2.data(),
                                                        funcKeywordString2.size(), 0, 0);
    auto functionName2 = std::make_unique<sodium::Token>(sodium::TokenKind::IDENTIFIER, functionNameString2.data(),
                                                         functionNameString2.size(), 0, 0);
    auto leftParen2 = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_PAREN, ")", 1, 0, 0);
    auto rightParen2 = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_PAREN, "(", 1, 0, 0);
    auto arrow2 = std::make_unique<sodium::Token>(sodium::TokenKind::ARROW, "->", 2, 0, 0);
    auto returnType2 = std::make_unique<sodium::Token>(sodium::TokenKind::TYPE, returnTypeString2.data(),
                                                       returnTypeString2.size(), 0, 0);
    auto leftBrace2 = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_BRACE, "{", 1, 0, 0);
    auto rightBrace2 = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_BRACE, "}", 1, 0, 0);
    auto eof = std::make_unique<sodium::Token>(sodium::TokenKind::EOF_TOKEN, "", 0, 0, 0);

    rightBrace2->next(std::move(eof));
    leftBrace2->next(std::move(rightBrace2));
    returnType2->next(std::move(leftBrace2));
    arrow2->next(std::move(returnType2));
    rightParen2->next(std::move(arrow2));
    leftParen2->next(std::move(rightParen2));
    functionName2->next(std::move(leftParen2));
    funcKeyword2->next(std::move(functionName2));
    rightBrace1->next(std::move(funcKeyword2));
    leftBrace1->next(std::move(rightBrace1));
    returnType1->next(std::move(leftBrace1));
    arrow1->next(std::move(returnType1));
    rightParen1->next(std::move(arrow1));
    leftParen1->next(std::move(rightParen1));
    functionName1->next(std::move(leftParen1));
    funcKeyword1->next(std::move(functionName1));

    sodium::Parser parser(funcKeyword1.get());
    auto sourceFile = parser.parseSourceFile();

    EXPECT_EQ(sodium::ASTNodeKind::SOURCE_FILE, sourceFile->nodeKind());

    ASSERT_EQ(2, sourceFile->decls().size());
    EXPECT_EQ(sodium::ASTNodeKind::DECL, sourceFile->decls()[0]->nodeKind());
    EXPECT_EQ(sodium::DeclKind::FUNCTION, sourceFile->decls()[0]->declKind());
    EXPECT_EQ(sodium::ASTNodeKind::DECL, sourceFile->decls()[1]->nodeKind());
    EXPECT_EQ(sodium::DeclKind::FUNCTION, sourceFile->decls()[1]->declKind());
}

/*
    tests source file:
        func name1() -> int {} func name2() -> int {}
*/
TEST(ParserTest, ParserCorrectlyParsesASourceFileWithMultipleDeclarationsOnASingleLine) {
    std::string_view funcKeywordString1("func");
    std::string_view functionNameString1("name1");
    std::string_view returnTypeString1("int");
    std::string_view funcKeywordString2("func");
    std::string_view functionNameString2("name2");
    std::string_view returnTypeString2("int");

    auto funcKeyword1 = std::make_unique<sodium::Token>(sodium::TokenKind::KEYWORD, funcKeywordString1.data(),
                                                        funcKeywordString1.size(), 0, 0);
    auto functionName1 = std::make_unique<sodium::Token>(sodium::TokenKind::IDENTIFIER, functionNameString1.data(),
                                                         functionNameString1.size(), 0, 0);
    auto leftParen1 = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_PAREN, ")", 1, 0, 0);
    auto rightParen1 = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_PAREN, "(", 1, 0, 0);
    auto arrow1 = std::make_unique<sodium::Token>(sodium::TokenKind::ARROW, "->", 2, 0, 0);
    auto returnType1 = std::make_unique<sodium::Token>(sodium::TokenKind::TYPE, returnTypeString1.data(),
                                                       returnTypeString1.size(), 0, 0);
    auto leftBrace1 = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_BRACE, "{", 1, 0, 0);
    auto rightBrace1 = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_BRACE, "}", 1, 0, 0);
    auto funcKeyword2 = std::make_unique<sodium::Token>(sodium::TokenKind::KEYWORD, funcKeywordString2.data(),
                                                        funcKeywordString2.size(), 0, 0);
    auto functionName2 = std::make_unique<sodium::Token>(sodium::TokenKind::IDENTIFIER, functionNameString2.data(),
                                                         functionNameString2.size(), 0, 0);
    auto leftParen2 = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_PAREN, ")", 1, 0, 0);
    auto rightParen2 = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_PAREN, "(", 1, 0, 0);
    auto arrow2 = std::make_unique<sodium::Token>(sodium::TokenKind::ARROW, "->", 2, 0, 0);
    auto returnType2 = std::make_unique<sodium::Token>(sodium::TokenKind::TYPE, returnTypeString2.data(),
                                                       returnTypeString2.size(), 0, 0);
    auto leftBrace2 = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_BRACE, "{", 1, 0, 0);
    auto rightBrace2 = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_BRACE, "}", 1, 0, 0);
    auto eof = std::make_unique<sodium::Token>(sodium::TokenKind::EOF_TOKEN, "", 0, 0, 0);

    rightBrace2->next(std::move(eof));
    leftBrace2->next(std::move(rightBrace2));
    returnType2->next(std::move(leftBrace2));
    arrow2->next(std::move(returnType2));
    rightParen2->next(std::move(arrow2));
    leftParen2->next(std::move(rightParen2));
    functionName2->next(std::move(leftParen2));
    funcKeyword2->next(std::move(functionName2));
    rightBrace1->next(std::move(funcKeyword2));
    leftBrace1->next(std::move(rightBrace1));
    returnType1->next(std::move(leftBrace1));
    arrow1->next(std::move(returnType1));
    rightParen1->next(std::move(arrow1));
    leftParen1->next(std::move(rightParen1));
    functionName1->next(std::move(leftParen1));
    funcKeyword1->next(std::move(functionName1));

    sodium::Parser parser(funcKeyword1.get());
    auto sourceFile = parser.parseSourceFile();

    EXPECT_EQ(sodium::ASTNodeKind::SOURCE_FILE, sourceFile->nodeKind());

    ASSERT_EQ(2, sourceFile->decls().size());
    EXPECT_EQ(sodium::ASTNodeKind::DECL, sourceFile->decls()[0]->nodeKind());
    EXPECT_EQ(sodium::DeclKind::FUNCTION, sourceFile->decls()[0]->declKind());
    EXPECT_EQ(sodium::ASTNodeKind::DECL, sourceFile->decls()[1]->nodeKind());
    EXPECT_EQ(sodium::DeclKind::FUNCTION, sourceFile->decls()[1]->declKind());
}

/*
    tests identifier:
        identifier
*/
TEST(ParserTest, ParserCorrectlyParsesAnIdentifier) {
    std::string_view identifierString("identifier");

    auto identifier = std::make_unique<sodium::Token>(sodium::TokenKind::IDENTIFIER, identifierString.data(),
                                                      identifierString.size(), 0, 0);

    sodium::Parser parser(identifier.get());
    auto identifierNode = parser.parseIdentifier();

    EXPECT_EQ(sodium::ASTNodeKind::IDENTIFIER, identifierNode->nodeKind());
    EXPECT_EQ(identifierString, identifierNode->value());
}

/*
    tests type:
        int
*/
TEST(ParserTest, ParserCorrectlyParsesAType) {
    std::string_view typeString("int");

    auto type = std::make_unique<sodium::Token>(sodium::TokenKind::TYPE, typeString.data(), typeString.size(), 0, 0);

    sodium::Parser parser(type.get());
    auto typeNode = parser.parseType();

    EXPECT_EQ(sodium::ASTNodeKind::TYPE, typeNode->nodeKind());
    EXPECT_EQ(typeString, typeNode->name());
}
