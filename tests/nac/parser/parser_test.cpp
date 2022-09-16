#include "sodium/nac/parser/parser.h"

#include <string_view>

#include "gtest/gtest.h"

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/decl.h"
#include "sodium/nac/ast/identifier.h"
#include "sodium/nac/ast/source_file.h"
#include "sodium/nac/ast/type.h"

/*
    tests source file:

*/
TEST(ParserTest, ParserCorrectlyParsesAnEmptySourceFile) {
    auto parser = sodium::Parser("");
    auto source_file = parser.parse_source_file();

    EXPECT_EQ(sodium::ASTNodeKind::SOURCE_FILE, source_file->node_kind());
    EXPECT_EQ(0, source_file->decls().size());
}

/*
    tests source file:
        func name() -> int {}
*/
TEST(ParserTest, ParserCorrectlyParsesASourceFileWithASingleDeclaration) {
    auto src = std::string_view("func name() -> int {}");

    auto parser = sodium::Parser(src);
    auto source_file = parser.parse_source_file();

    EXPECT_EQ(sodium::ASTNodeKind::SOURCE_FILE, source_file->node_kind());

    ASSERT_EQ(1, source_file->decls().size());
    EXPECT_EQ(sodium::ASTNodeKind::DECL, source_file->decls()[0]->node_kind());
    EXPECT_EQ(sodium::DeclKind::FUNC, source_file->decls()[0]->decl_kind());
}

/*
    tests source file:
        func name1() -> int {}
        func name2() -> int {}
*/
TEST(ParserTest, ParserCorrectlyParsesASourceFileWithMultipleDeclarations) {
    auto src = std::string_view("func name1() -> int {}\n"
                                "func name2() -> int {}");

    auto parser = sodium::Parser(src);
    auto source_file = parser.parse_source_file();

    EXPECT_EQ(sodium::ASTNodeKind::SOURCE_FILE, source_file->node_kind());

    ASSERT_EQ(2, source_file->decls().size());
    EXPECT_EQ(sodium::ASTNodeKind::DECL, source_file->decls()[0]->node_kind());
    EXPECT_EQ(sodium::DeclKind::FUNC, source_file->decls()[0]->decl_kind());
    EXPECT_EQ(sodium::ASTNodeKind::DECL, source_file->decls()[1]->node_kind());
    EXPECT_EQ(sodium::DeclKind::FUNC, source_file->decls()[1]->decl_kind());
}

/*
    tests identifier:
        identifier
*/
TEST(ParserTest, ParserCorrectlyParsesAnIdentifier) {
    auto identifier_string = std::string_view("identifier");

    auto parser = sodium::Parser(identifier_string);
    auto identifier_node = parser.parse_identifier();

    EXPECT_EQ(sodium::ASTNodeKind::IDENTIFIER, identifier_node->node_kind());
    EXPECT_EQ(identifier_string, identifier_node->value());
}

/*
    tests type:
        int
*/
TEST(ParserTest, ParserCorrectlyParsesAType) {
    auto type_string = std::string_view("int");

    auto parser = sodium::Parser(type_string);
    auto type_node = parser.parse_type();

    EXPECT_EQ(sodium::ASTNodeKind::TYPE, type_node->node_kind());
    EXPECT_EQ(type_string, type_node->name());
}
