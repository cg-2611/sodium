#include "sodium/nac/parser/parser.h"

#include <memory>
#include <string_view>
#include <utility>

#include <gtest/gtest.h>

#include "sodium/nac/ast/expr.h"
#include "sodium/nac/ast/stmt.h"
#include "sodium/nac/lexer/token.h"

/*
    tests statement:
        return 2;
*/
TEST(ParseStmtTest, ParserCorrectlyDispatchesToAReturnStatement) {
    sodium::Parser parser("return 2;");
    auto stmt(parser.parseStmt());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, stmt->nodeKind());
    EXPECT_EQ(sodium::StmtKind::RETURN, stmt->stmtKind());
}

/*
    tests block:
        {}
*/
TEST(ParseStmtTest, ParserCorrectlyDispatchesToABlockStatement) {
    sodium::Parser parser("{}");
    auto block(parser.parseStmt());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->nodeKind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmtKind());
}

/*
    tests block:
        {}
*/
TEST(ParseStmtTest, ParserCorrectlyParsesAnEmptyBlock) {
    sodium::Parser parser("{}");
    auto block(parser.parseBlock());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->nodeKind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmtKind());
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
    auto block(parser.parseBlock());

    ASSERT_EQ(1, block->stmts().size());

    auto *nestedBlock = dynamic_cast<sodium::Block *>(block->stmts()[0].get());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->nodeKind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmtKind());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, nestedBlock->nodeKind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, nestedBlock->stmtKind());
    EXPECT_EQ(0, nestedBlock->stmts().size());
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
    auto block(parser.parseBlock());

    ASSERT_EQ(2, block->stmts().size());

    auto *nestedBlock1 = dynamic_cast<sodium::Block *>(block->stmts()[0].get());
    auto *nestedBlock2 = dynamic_cast<sodium::Block *>(block->stmts()[1].get());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->nodeKind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmtKind());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, nestedBlock1->nodeKind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, nestedBlock1->stmtKind());
    EXPECT_EQ(0, nestedBlock1->stmts().size());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, nestedBlock2->nodeKind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, nestedBlock2->stmtKind());
    EXPECT_EQ(0, nestedBlock2->stmts().size());
}

/*
    tests block:
        { return 0; }
*/
TEST(ParseStmtTest, ParserCorrectlyParsesABlockWithASingleStatement) {
    sodium::Parser parser("{ return 0; }");
    auto block(parser.parseBlock());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->nodeKind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmtKind());
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
    auto block(parser.parseBlock());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->nodeKind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmtKind());
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
    auto block(parser.parseBlock());

    ASSERT_EQ(2, block->stmts().size());

    auto *stmt = block->stmts()[0].get();
    auto *nestedBlock = dynamic_cast<sodium::Block *>(block->stmts()[1].get());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->nodeKind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmtKind());

    EXPECT_EQ(sodium::StmtKind::RETURN, stmt->stmtKind());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, nestedBlock->nodeKind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, nestedBlock->stmtKind());

    ASSERT_EQ(1, nestedBlock->stmts().size());
    auto *nestedStmt = nestedBlock->stmts()[0].get();
    EXPECT_EQ(sodium::StmtKind::RETURN, nestedStmt->stmtKind());
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
    auto block(parser.parseBlock());

    ASSERT_EQ(2, block->stmts().size());

    auto *stmt = block->stmts()[0].get();
    auto *nestedBlock = dynamic_cast<sodium::Block *>(block->stmts()[1].get());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->nodeKind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmtKind());

    EXPECT_EQ(sodium::StmtKind::RETURN, stmt->stmtKind());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, nestedBlock->nodeKind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, nestedBlock->stmtKind());

    ASSERT_EQ(2, nestedBlock->stmts().size());
    auto *nestedStmt1 = nestedBlock->stmts()[0].get();
    auto *nestedStmt2 = nestedBlock->stmts()[1].get();

    EXPECT_EQ(sodium::StmtKind::RETURN, nestedStmt1->stmtKind());
    EXPECT_EQ(sodium::StmtKind::RETURN, nestedStmt2->stmtKind());
}

/*
    tests return statement:
        return 2;
*/
TEST(ParseStmtTest, ParserCorrectlyParsesReturnStmtWithSingleDigit) {
    sodium::Parser parser("return 2;");
    auto returnStmt(parser.parseReturnStmt());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, returnStmt->nodeKind());
    EXPECT_EQ(sodium::StmtKind::RETURN, returnStmt->stmtKind());

    auto *returnExpr = dynamic_cast<sodium::NumericLiteralExpr *>(returnStmt->expr());

    EXPECT_EQ(2, returnExpr->value());
}

/*
    tests return statement:
        return 567;
*/
TEST(ParseStmtTest, ParserCorrectlyParsesReturnStmtWithMultipleDigit) {
    sodium::Parser parser("return 567;");
    auto returnStmt(parser.parseReturnStmt());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, returnStmt->nodeKind());
    EXPECT_EQ(sodium::StmtKind::RETURN, returnStmt->stmtKind());

    auto *returnExpr = dynamic_cast<sodium::NumericLiteralExpr *>(returnStmt->expr());

    EXPECT_EQ(567, returnExpr->value());
}
