#include "sodium/nac/parser/parser.h"

#include <memory>
#include <string_view>
#include <utility>

#include <gtest/gtest.h>

#include "sodium/nac/ast/expr.h"
#include "sodium/nac/ast/stmt.h"
#include "sodium/nac/token/token.h"

/*
    tests statement:
        return 2;
*/
TEST(ParseStmtTest, ParserCorrectlyDispatchesToAReturnStatement) {
    sodium::Parser parser("return 2;");
    auto stmt(parser.parse_stmt());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, stmt->node_kind());
    EXPECT_EQ(sodium::StmtKind::RETURN, stmt->stmt_kind());
}

/*
    tests block:
        {}
*/
TEST(ParseStmtTest, ParserCorrectlyDispatchesToABlockStatement) {
    sodium::Parser parser("{}");
    auto block(parser.parse_stmt());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmt_kind());
}

/*
    tests block:
        {}
*/
TEST(ParseStmtTest, ParserCorrectlyParsesAnEmptyBlock) {
    sodium::Parser parser("{}");
    auto block(parser.parse_block());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmt_kind());
    EXPECT_EQ(0, block->stmts().size());
}

/*
    tests block:
        {
            {}
        }
*/
TEST(ParseStmtTest, ParserCorrectlyParsesABlockWithAnEmptyNestedBlock) {
    std::string_view src("{\n"
                         "   {}\n"
                         "}");

    sodium::Parser parser(src);
    auto block(parser.parse_block());

    ASSERT_EQ(1, block->stmts().size());

    auto *nested_block = dynamic_cast<sodium::Block *>(block->stmts()[0].get());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmt_kind());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, nested_block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, nested_block->stmt_kind());
    EXPECT_EQ(0, nested_block->stmts().size());
}

/*
    tests block:
        {
            {}
            {}
        }
*/
TEST(ParseStmtTest, ParserCorrectlyParsesABlockWithMultipleEmptyNestedBlocks) {
    std::string_view src("{\n"
                         "   {}\n"
                         "   {}\n"
                         "}");

    sodium::Parser parser(src);
    auto block(parser.parse_block());

    ASSERT_EQ(2, block->stmts().size());

    auto *nested_block1 = dynamic_cast<sodium::Block *>(block->stmts()[0].get());
    auto *nested_block2 = dynamic_cast<sodium::Block *>(block->stmts()[1].get());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmt_kind());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, nested_block1->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, nested_block1->stmt_kind());
    EXPECT_EQ(0, nested_block1->stmts().size());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, nested_block2->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, nested_block2->stmt_kind());
    EXPECT_EQ(0, nested_block2->stmts().size());
}

/*
    tests block:
        { return 0; }
*/
TEST(ParseStmtTest, ParserCorrectlyParsesABlockWithASingleStatement) {
    sodium::Parser parser("{ return 0; }");
    auto block(parser.parse_block());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmt_kind());
    EXPECT_EQ(1, block->stmts().size());
}

/*
    tests block:
        {
            return 1;
            return 2;
        }
*/
TEST(ParseStmtTest, ParserCorrectlyParsesABlockWithMultipleStatements) {
    std::string_view src("{\n"
                         "   return 1;\n"
                         "   return 2;\n"
                         "}");

    sodium::Parser parser(src);
    auto block(parser.parse_block());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmt_kind());
    EXPECT_EQ(2, block->stmts().size());
}

/*
    tests block:
        {
            return 1;
            { return 2; }
        }
*/
TEST(ParseStmtTest, ParserCorrectlyParsesABlockWithAStatementAndANestedBlockWithAStatement) {
    std::string_view src("{\n"
                         "   return 1;\n"
                         "   { return 2; }\n"
                         "}");

    sodium::Parser parser(src);
    auto block(parser.parse_block());

    ASSERT_EQ(2, block->stmts().size());

    auto *stmt = block->stmts()[0].get();
    auto *nested_block = dynamic_cast<sodium::Block *>(block->stmts()[1].get());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmt_kind());

    EXPECT_EQ(sodium::StmtKind::RETURN, stmt->stmt_kind());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, nested_block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, nested_block->stmt_kind());

    ASSERT_EQ(1, nested_block->stmts().size());
    auto *nested_stmt = nested_block->stmts()[0].get();
    EXPECT_EQ(sodium::StmtKind::RETURN, nested_stmt->stmt_kind());
}

/*
    tests block:
        {
            return 1;
            {
                return 2;
                return 3;
            }
        }
*/
TEST(ParseStmtTest, ParserCorrectlyParsesAMultipleStatementBlockWithAMultiLineNestedBlockWithMultipleStatements) {
    std::string_view src("{\n"
                         "   return 1;\n"
                         "   {\n"
                         "       return 2;\n"
                         "       return 3;\n"
                         "   }\n"
                         "}");

    sodium::Parser parser(src);
    auto block(parser.parse_block());

    ASSERT_EQ(2, block->stmts().size());

    auto *stmt = block->stmts()[0].get();
    auto *nested_block = dynamic_cast<sodium::Block *>(block->stmts()[1].get());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmt_kind());

    EXPECT_EQ(sodium::StmtKind::RETURN, stmt->stmt_kind());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, nested_block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, nested_block->stmt_kind());

    ASSERT_EQ(2, nested_block->stmts().size());
    auto *nested_stmt1 = nested_block->stmts()[0].get();
    auto *nested_stmt2 = nested_block->stmts()[1].get();

    EXPECT_EQ(sodium::StmtKind::RETURN, nested_stmt1->stmt_kind());
    EXPECT_EQ(sodium::StmtKind::RETURN, nested_stmt2->stmt_kind());
}

/*
    tests return statement:
        return 2;
*/
TEST(ParseStmtTest, ParserCorrectlyParsesReturnStmtWithSingleDigit) {
    sodium::Parser parser("return 2;");
    auto return_stmt(parser.parse_return_stmt());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, return_stmt->node_kind());
    EXPECT_EQ(sodium::StmtKind::RETURN, return_stmt->stmt_kind());

    auto *return_expr = dynamic_cast<sodium::NumericLiteralExpr *>(return_stmt->expr());

    EXPECT_EQ(2, return_expr->value());
}

/*
    tests return statement:
        return 567;
*/
TEST(ParseStmtTest, ParserCorrectlyParsesReturnStmtWithMultipleDigit) {
    sodium::Parser parser("return 567;");
    auto return_stmt(parser.parse_return_stmt());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, return_stmt->node_kind());
    EXPECT_EQ(sodium::StmtKind::RETURN, return_stmt->stmt_kind());

    auto *return_expr = dynamic_cast<sodium::NumericLiteralExpr *>(return_stmt->expr());

    EXPECT_EQ(567, return_expr->value());
}
