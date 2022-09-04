#include "sodium/nac/parser/parser.h"

#include <string_view>

#include <gtest/gtest.h>

#include "sodium/nac/ast/identifier.h"
#include "sodium/nac/ast/source_file.h"
#include "sodium/nac/ast/type.h"
#include "sodium/nac/lexer/token.h"

/*
    tests source file:

*/
TEST(ParserTest, ParserCorrectlyParsesAnEmptySourceFile) {
    sodium::Parser parser("");
    auto sourceFile = parser.parseSourceFile();

    EXPECT_EQ(sodium::ASTNodeKind::SOURCE_FILE, sourceFile->nodeKind());
    EXPECT_EQ(0, sourceFile->decls().size());
}

/*
    tests source file:
        func name() -> int {}
*/
TEST(ParserTest, ParserCorrectlyParsesASourceFileWithASingleDeclaration) {
    std::string_view src("func name() -> int {}");

    sodium::Parser parser(src);
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
TEST(ParserTest, ParserCorrectlyParsesASourceFileWithMultipleDeclarations) {
    std::string_view src("func name1() -> int {}\n"
                         "func name2() -> int {}");

    sodium::Parser parser(src);
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
    sodium::Parser parser(identifierString);
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

    sodium::Parser parser(typeString);
    auto typeNode = parser.parseType();

    EXPECT_EQ(sodium::ASTNodeKind::TYPE, typeNode->nodeKind());
    EXPECT_EQ(typeString, typeNode->name());
}
