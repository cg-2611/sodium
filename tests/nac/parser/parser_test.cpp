#include "sodium/nac/parser/parser.h"

#include <string_view>

#include <gtest/gtest.h>

#include "sodium/nac/ast/identifier.h"
#include "sodium/nac/ast/source_file.h"
#include "sodium/nac/ast/type.h"
#include "sodium/nac/token/token.h"

/*
    tests source file:

*/
TEST(ParserTest, ParserCorrectlyParsesAnEmptySourceFile) {
    sodium::Parser parser("");
    auto source_file = parser.parse_source_file();

    EXPECT_EQ(sodium::ASTNodeKind::SOURCE_FILE, source_file->node_kind());
    EXPECT_EQ(0, source_file->decls().size());
}

/*
    tests source file:
        func name() -> int {}
*/
TEST(ParserTest, ParserCorrectlyParsesASourceFileWithASingleDeclaration) {
    std::string_view src("func name() -> int {}");

    sodium::Parser parser(src);
    auto source_file = parser.parse_source_file();

    EXPECT_EQ(sodium::ASTNodeKind::SOURCE_FILE, source_file->node_kind());

    ASSERT_EQ(1, source_file->decls().size());
    EXPECT_EQ(sodium::ASTNodeKind::DECL, source_file->decls()[0]->node_kind());
    EXPECT_EQ(sodium::DeclKind::FUNCTION, source_file->decls()[0]->decl_kind());
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
    auto source_file = parser.parse_source_file();

    EXPECT_EQ(sodium::ASTNodeKind::SOURCE_FILE, source_file->node_kind());

    ASSERT_EQ(2, source_file->decls().size());
    EXPECT_EQ(sodium::ASTNodeKind::DECL, source_file->decls()[0]->node_kind());
    EXPECT_EQ(sodium::DeclKind::FUNCTION, source_file->decls()[0]->decl_kind());
    EXPECT_EQ(sodium::ASTNodeKind::DECL, source_file->decls()[1]->node_kind());
    EXPECT_EQ(sodium::DeclKind::FUNCTION, source_file->decls()[1]->decl_kind());
}

/*
    tests identifier:
        identifier
*/
TEST(ParserTest, ParserCorrectlyParsesAnIdentifier) {
    std::string_view identifier_string("identifier");
    sodium::Parser parser(identifier_string);
    auto identifier_node = parser.parse_identifier();

    EXPECT_EQ(sodium::ASTNodeKind::IDENTIFIER, identifier_node->node_kind());
    EXPECT_EQ(identifier_string, identifier_node->value());
}

/*
    tests type:
        int
*/
TEST(ParserTest, ParserCorrectlyParsesAType) {
    std::string_view type_string("int");

    sodium::Parser parser(type_string);
    auto type_node = parser.parse_type();

    EXPECT_EQ(sodium::ASTNodeKind::TYPE, type_node->node_kind());
    EXPECT_EQ(type_string, type_node->name());
}
