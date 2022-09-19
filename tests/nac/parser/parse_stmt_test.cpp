#include "sodium/nac/parser/parser.h"

#include <string_view>

#include "gtest/gtest.h"

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/expr.h"
#include "sodium/nac/ast/stmt.h"
#include "sodium/nac/diagnostics/diagnostic_engine.h"
#include "sodium/nac/lexer/lexer.h"
#include "sodium/nac/parser/parser_diagnostics.h"

/*
    tests statement:
        {}
*/
TEST(ParseStmtTest, ParserCorrectlyDispatchesToABlockStatement) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("{}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto stmt = parser.parse_stmt();

    EXPECT_EQ(sodium::ASTNodeKind::STMT, stmt->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, stmt->stmt_kind());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

/*
    tests statement:
        return 2;
*/
TEST(ParseStmtTest, ParserCorrectlyDispatchesToAReturnStatement) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("return 2;");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto stmt = parser.parse_stmt();

    EXPECT_EQ(sodium::ASTNodeKind::STMT, stmt->node_kind());
    EXPECT_EQ(sodium::StmtKind::RETURN, stmt->stmt_kind());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

/*
    tests statement:
        func
*/
TEST(ParseStmtTest, ParserDiagnosesParserErrorForInvalidStatement) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("func");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto stmt = parser.parse_stmt();

    EXPECT_EQ(nullptr, stmt);

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_STATEMENT, parser_error->kind());
}

/*
    tests block:
        {}
*/
TEST(ParseStmtTest, ParserCorrectlyParsesAnEmptyBlock) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("{}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto block = parser.parse_block();

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmt_kind());
    EXPECT_EQ(0, block->stmts().size());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

/*
    tests block:
        {
            {}
        }
*/
TEST(ParseStmtTest, ParserCorrectlyParsesABlockWithAnEmptyNestedBlock) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("{\n"
                                "    {}\n"
                                "}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto block = parser.parse_block();

    ASSERT_EQ(1, block->stmts().size());

    auto *nested_block = dynamic_cast<sodium::Block *>(block->stmts()[0].get());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmt_kind());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, nested_block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, nested_block->stmt_kind());
    EXPECT_EQ(0, nested_block->stmts().size());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

/*
    tests block:
        {
            {}
            {}
        }
*/
TEST(ParseStmtTest, ParserCorrectlyParsesABlockWithMultipleEmptyNestedBlocks) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("{\n"
                                "    {}\n"
                                "    {}\n"
                                "}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto block = parser.parse_block();

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmt_kind());

    ASSERT_EQ(2, block->stmts().size());

    auto *nested_block1 = dynamic_cast<sodium::Block *>(block->stmts()[0].get());
    EXPECT_EQ(sodium::ASTNodeKind::STMT, nested_block1->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, nested_block1->stmt_kind());
    EXPECT_EQ(0, nested_block1->stmts().size());

    auto *nested_block2 = dynamic_cast<sodium::Block *>(block->stmts()[1].get());
    EXPECT_EQ(sodium::ASTNodeKind::STMT, nested_block2->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, nested_block2->stmt_kind());
    EXPECT_EQ(0, nested_block2->stmts().size());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

/*
    tests block:
        { return 0; }
*/
TEST(ParseStmtTest, ParserCorrectlyParsesABlockWithASingleStatement) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("{ return 0; }");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto block = parser.parse_block();

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmt_kind());
    EXPECT_EQ(1, block->stmts().size());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

/*
    tests block:
        {
            return 1;
            return 2;
        }
*/
TEST(ParseStmtTest, ParserCorrectlyParsesABlockWithMultipleStatements) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("{\n"
                                "    return 1;\n"
                                "    return 2;\n"
                                "}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto block = parser.parse_block();

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmt_kind());
    EXPECT_EQ(2, block->stmts().size());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

/*
    tests block:
        {
            return 1;
            { return 2; }
        }
*/
TEST(ParseStmtTest, ParserCorrectlyParsesABlockWithAStatementAndANestedBlockWithAStatement) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("{\n"
                                "    return 1;\n"
                                "     { return 2; }\n"
                                "}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto block = parser.parse_block();

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmt_kind());

    ASSERT_EQ(2, block->stmts().size());

    auto *stmt = block->stmts()[0].get();
    EXPECT_EQ(sodium::StmtKind::RETURN, stmt->stmt_kind());

    auto *nested_block = dynamic_cast<sodium::Block *>(block->stmts()[1].get());
    EXPECT_EQ(sodium::ASTNodeKind::STMT, nested_block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, nested_block->stmt_kind());

    ASSERT_EQ(1, nested_block->stmts().size());

    auto *nested_stmt = nested_block->stmts()[0].get();
    EXPECT_EQ(sodium::StmtKind::RETURN, nested_stmt->stmt_kind());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
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
TEST(ParseStmtTest, ParserCorrectlyParsesAMultipleStatementBlockWithANestedBlockWithMultipleStatements) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("{\n"
                                "    return 1;\n"
                                "    {\n"
                                "        return 2;\n"
                                "        return 3;\n"
                                "    }\n"
                                "}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto block = parser.parse_block();

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmt_kind());

    ASSERT_EQ(2, block->stmts().size());

    auto *stmt = block->stmts()[0].get();
    EXPECT_EQ(sodium::StmtKind::RETURN, stmt->stmt_kind());

    auto *nested_block = dynamic_cast<sodium::Block *>(block->stmts()[1].get());
    EXPECT_EQ(sodium::ASTNodeKind::STMT, nested_block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, nested_block->stmt_kind());

    ASSERT_EQ(2, nested_block->stmts().size());

    auto *nested_stmt1 = nested_block->stmts()[0].get();
    EXPECT_EQ(sodium::StmtKind::RETURN, nested_stmt1->stmt_kind());

    auto *nested_stmt2 = nested_block->stmts()[1].get();
    EXPECT_EQ(sodium::StmtKind::RETURN, nested_stmt2->stmt_kind());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

/*
    tests block:
        {
*/
TEST(ParseStmtTest, ParserDiagnosesParserErrorForMissingRightBraceForBlock) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("{");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto block = parser.parse_block();

    EXPECT_EQ(nullptr, block);

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_RIGHT_BRACE_BLOCK, parser_error->kind());
}

/*
    tests block:
        }
*/
TEST(ParseStmtTest, ParserDiagnosesParserErrorForMissingLeftBraceForBlock) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto block = parser.parse_block();

    EXPECT_EQ(nullptr, block);

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_LEFT_BRACE_BLOCK, parser_error->kind());
}

/*
    tests block:
        {
            func
        }
*/
TEST(ParseStmtTest, ParserIgnoresInvalidStatementsInABlockAndDiagnosesParserError) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("{\n"
                                "    func\n"
                                "}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto block = parser.parse_block();

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmt_kind());

    EXPECT_EQ(0, block->stmts().size());

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_STATEMENT, parser_error->kind());
}

/*
    tests block:
    {
        func
        return 1;
        return 2;
    }
*/
TEST(ParseStmtTest, ParserRecoversAfterDiagnosingParserErrorWhenParsingABlock1) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("{\n"
                                "    func\n"
                                "    return 1;"
                                "    return 2;"
                                "}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto block = parser.parse_block();

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmt_kind());

    ASSERT_EQ(1, block->stmts().size());
    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->stmts()[0]->node_kind());
    EXPECT_EQ(sodium::StmtKind::RETURN, block->stmts()[0]->stmt_kind());

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_STATEMENT, parser_error->kind());
}

/*
    tests block:
    {
        func
        {
            return 1;
        }
    }
*/
TEST(ParseStmtTest, ParserRecoversAfterDiagnosingParserErrorWhenParsingABlock2) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("{\n"
                                "    func\n"
                                "    {\n"
                                "        return 1;\n"
                                "    }\n"
                                "}");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto block = parser.parse_block();

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmt_kind());

    ASSERT_EQ(1, block->stmts().size());
    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->stmts()[0]->node_kind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmts()[0]->stmt_kind());

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_STATEMENT, parser_error->kind());
}

/*
    tests return statement:
        return 2;
*/
TEST(ParseStmtTest, ParserCorrectlyParsesReturnStmtWithSingleDigit) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("return 2;");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto return_stmt = parser.parse_return_stmt();

    EXPECT_EQ(sodium::ASTNodeKind::STMT, return_stmt->node_kind());
    EXPECT_EQ(sodium::StmtKind::RETURN, return_stmt->stmt_kind());

    auto *return_expr = dynamic_cast<sodium::IntegerLiteralExpr *>(return_stmt->expr());
    EXPECT_EQ(2, return_expr->value());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

/*
    tests return statement:
        return 567;
*/
TEST(ParseStmtTest, ParserCorrectlyParsesReturnStmtWithMultipleDigit) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("return 567;");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto return_stmt = parser.parse_return_stmt();

    EXPECT_EQ(sodium::ASTNodeKind::STMT, return_stmt->node_kind());
    EXPECT_EQ(sodium::StmtKind::RETURN, return_stmt->stmt_kind());

    auto *return_expr = dynamic_cast<sodium::IntegerLiteralExpr *>(return_stmt->expr());
    EXPECT_EQ(567, return_expr->value());

    EXPECT_FALSE(diagnostics.has_problems());
    EXPECT_EQ(0, diagnostics.count());
}

/*
    tests return statement:
        func 2;
*/
TEST(ParseStmtTest, ParserDiagnosesParserErrorForMissingReturnKeywordInReturnStatement) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("func 2;");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto return_stmt = parser.parse_return_stmt();

    EXPECT_EQ(nullptr, return_stmt);

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_KEYWORD_RETURN, parser_error->kind());
}

/*
    tests return statement:
        return;
*/
TEST(ParseStmtTest, ParserDiagnosesParserErrorForMissingExpressionInReturnStatement) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("return;");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto return_stmt = parser.parse_return_stmt();

    EXPECT_EQ(nullptr, return_stmt);

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_EXPRESSION, parser_error->kind());
}

/*
    tests return statement:
        return 2
*/
TEST(ParseStmtTest, ParserDiagnosesParserErrorForMissingSemicolonInReturnStatement) {
    auto diagnostics = sodium::DiagnosticEngine();

    auto src = std::string_view("return 2");
    auto token_buffer = sodium::Lexer(src, diagnostics).tokenize();

    auto parser = sodium::Parser(token_buffer, diagnostics);
    auto return_stmt = parser.parse_return_stmt();

    EXPECT_EQ(nullptr, return_stmt);

    EXPECT_TRUE(diagnostics.has_problems());
    EXPECT_EQ(1, diagnostics.count_errors());

    ASSERT_EQ(1, diagnostics.count());

    auto *diagnostic = diagnostics.get(0);
    ASSERT_NE(nullptr, diagnostic);

    auto *parser_error = dynamic_cast<sodium::ParserError *>(diagnostic);
    EXPECT_EQ(sodium::DiagnosticKind::ERROR, parser_error->diagnostic_kind());
    EXPECT_EQ(sodium::ParserErrorKind::EXPECTED_SEMICOLON, parser_error->kind());
}
