#include "sodium/nac/parser/parser.h"

#include <memory>
#include <string_view>
#include <utility>

#include <gtest/gtest.h>

#include "sodium/nac/ast/expr.h"
#include "sodium/nac/ast/stmt.h"
#include "sodium/nac/lexer/token.h"

/*
    tests return statement:
        return 2;
*/
TEST(ParseStmtTest, ParserCorrectlyDispatchesToAReturnStatement) {
    std::string_view returnKeywordString("return");
    std::string_view returnValueString("2");

    auto returnKeyword = std::make_unique<sodium::Token>(sodium::TokenKind::KEYWORD, returnKeywordString.data(),
                                                         returnKeywordString.size(), 0, 0);
    auto returnValue = std::make_unique<sodium::Token>(sodium::TokenKind::NUMERIC_LITERAL, returnValueString.data(),
                                                       returnValueString.size(), 0, 0);
    auto semicolon = std::make_unique<sodium::Token>(sodium::TokenKind::SEMICOLON, ";", 1, 0, 0);

    returnValue->next(std::move(semicolon));
    returnKeyword->next(std::move(returnValue));

    sodium::Parser parser(returnKeyword.get());
    auto returnStmt(parser.parseStmt());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, returnStmt->nodeKind());
    EXPECT_EQ(sodium::StmtKind::RETURN, returnStmt->stmtKind());
}

/*
    tests block:
        {}
*/
TEST(ParseStmtTest, ParserCorrectlyDispatchesToABlockStatement) {
    auto leftBrace = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_BRACE, "{", 1, 0, 0);
    auto rightBrace = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_BRACE, "}", 1, 0, 0);

    leftBrace->next(std::move(rightBrace));

    sodium::Parser parser(leftBrace.get());
    auto block(parser.parseStmt());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->nodeKind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmtKind());
}

/*
    tests block:
        {}
*/
TEST(ParseStmtTest, ParserCorrectlyParsesAnEmptyBlock) {
    auto leftBrace = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_BRACE, "{", 1, 0, 0);
    auto rightBrace = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_BRACE, "}", 1, 0, 0);

    leftBrace->next(std::move(rightBrace));

    sodium::Parser parser(leftBrace.get());
    auto block(parser.parseBlock());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->nodeKind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmtKind());
    EXPECT_EQ(0, block->stmts().size());
}

/*
    tests block:
        {{}}
*/
TEST(ParseStmtTest, ParserCorrectlyParsesABlockWithAnEmptyNestedBlock) {
    auto leftBrace = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_BRACE, "{", 1, 0, 0);
    auto nestedLeftBrace = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_BRACE, "{", 1, 0, 0);
    auto nestedRightBrace = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_BRACE, "}", 1, 0, 0);
    auto rightBrace = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_BRACE, "}", 1, 0, 0);

    nestedRightBrace->next(std::move(rightBrace));
    nestedLeftBrace->next(std::move(nestedRightBrace));
    leftBrace->next(std::move(nestedLeftBrace));

    sodium::Parser parser(leftBrace.get());
    auto block(parser.parseBlock());

    ASSERT_EQ(1, block->stmts().size());

    auto nestedBlock = static_cast<sodium::Block *>(block->stmts()[0].get());

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
    auto leftBrace = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_BRACE, "{", 1, 0, 0);
    auto nestedLeftBrace1 = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_BRACE, "{", 1, 0, 0);
    auto nestedRightBrace1 = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_BRACE, "}", 1, 0, 0);
    auto nestedLeftBrace2 = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_BRACE, "{", 1, 0, 0);
    auto nestedRightBrace2 = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_BRACE, "}", 1, 0, 0);
    auto rightBrace = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_BRACE, "}", 1, 0, 0);

    nestedRightBrace2->next(std::move(rightBrace));
    nestedLeftBrace2->next(std::move(nestedRightBrace2));
    nestedRightBrace1->next(std::move(nestedLeftBrace2));
    nestedLeftBrace1->next(std::move(nestedRightBrace1));
    leftBrace->next(std::move(nestedLeftBrace1));

    sodium::Parser parser(leftBrace.get());
    auto block(parser.parseBlock());

    ASSERT_EQ(2, block->stmts().size());

    auto nestedBlock1 = static_cast<sodium::Block *>(block->stmts()[0].get());
    auto nestedBlock2 = static_cast<sodium::Block *>(block->stmts()[1].get());

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
    std::string_view returnKeywordString("return");
    std::string_view returnValueString("0");

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

    sodium::Parser parser(leftBrace.get());
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
    std::string_view returnKeywordString1("return");
    std::string_view returnValueString1("1");
    std::string_view returnKeywordString2("return");
    std::string_view returnValueString2("2");

    auto leftBrace = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_BRACE, "{", 1, 0, 0);
    auto returnKeyword1 = std::make_unique<sodium::Token>(sodium::TokenKind::KEYWORD, returnKeywordString1.data(),
                                                          returnKeywordString1.size(), 0, 0);
    auto returnValue1 = std::make_unique<sodium::Token>(sodium::TokenKind::NUMERIC_LITERAL, returnValueString1.data(),
                                                        returnValueString1.size(), 0, 0);
    auto semicolon1 = std::make_unique<sodium::Token>(sodium::TokenKind::SEMICOLON, ";", 1, 0, 0);
    auto returnKeyword2 = std::make_unique<sodium::Token>(sodium::TokenKind::KEYWORD, returnKeywordString2.data(),
                                                          returnKeywordString2.size(), 0, 0);
    auto returnValue2 = std::make_unique<sodium::Token>(sodium::TokenKind::NUMERIC_LITERAL, returnValueString2.data(),
                                                        returnValueString2.size(), 0, 0);
    auto semicolon2 = std::make_unique<sodium::Token>(sodium::TokenKind::SEMICOLON, ";", 1, 0, 0);
    auto rightBrace = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_BRACE, "}", 1, 0, 0);

    semicolon2->next(std::move(rightBrace));
    returnValue2->next(std::move(semicolon2));
    returnKeyword2->next(std::move(returnValue2));
    semicolon1->next(std::move(returnKeyword2));
    returnValue1->next(std::move(semicolon1));
    returnKeyword1->next(std::move(returnValue1));
    leftBrace->next(std::move(returnKeyword1));

    sodium::Parser parser(leftBrace.get());
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
    std::string_view returnKeywordString("return");
    std::string_view returnValueString("1");
    std::string_view nestedReturnKeywordString("return");
    std::string_view nestedReturnValueString("2");

    auto leftBrace = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_BRACE, "{", 1, 0, 0);
    auto returnKeyword = std::make_unique<sodium::Token>(sodium::TokenKind::KEYWORD, returnKeywordString.data(),
                                                         returnKeywordString.size(), 0, 0);
    auto returnValue = std::make_unique<sodium::Token>(sodium::TokenKind::NUMERIC_LITERAL, returnValueString.data(),
                                                       returnValueString.size(), 0, 0);
    auto semicolon1 = std::make_unique<sodium::Token>(sodium::TokenKind::SEMICOLON, ";", 1, 0, 0);
    auto nestedLeftBrace = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_BRACE, "{", 1, 0, 0);
    auto nestedReturnKeyword = std::make_unique<sodium::Token>(
        sodium::TokenKind::KEYWORD, nestedReturnKeywordString.data(), nestedReturnKeywordString.size(), 0, 0);
    auto nestedReturnValue = std::make_unique<sodium::Token>(
        sodium::TokenKind::NUMERIC_LITERAL, nestedReturnValueString.data(), nestedReturnValueString.size(), 0, 0);
    auto semicolon2 = std::make_unique<sodium::Token>(sodium::TokenKind::SEMICOLON, ";", 1, 0, 0);
    auto nestedRightBrace = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_BRACE, "}", 1, 0, 0);
    auto rightBrace = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_BRACE, "}", 1, 0, 0);

    nestedRightBrace->next(std::move(rightBrace));
    semicolon2->next(std::move(nestedRightBrace));
    nestedReturnValue->next(std::move(semicolon2));
    nestedReturnKeyword->next(std::move(nestedReturnValue));
    nestedLeftBrace->next(std::move(nestedReturnKeyword));
    semicolon1->next(std::move(nestedLeftBrace));
    returnValue->next(std::move(semicolon1));
    returnKeyword->next(std::move(returnValue));
    leftBrace->next(std::move(returnKeyword));

    sodium::Parser parser(leftBrace.get());
    auto block(parser.parseBlock());

    ASSERT_EQ(2, block->stmts().size());

    auto stmt = block->stmts()[0].get();
    auto nestedBlock = static_cast<sodium::Block *>(block->stmts()[1].get());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->nodeKind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmtKind());

    EXPECT_EQ(sodium::StmtKind::RETURN, stmt->stmtKind());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, nestedBlock->nodeKind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, nestedBlock->stmtKind());

    ASSERT_EQ(1, nestedBlock->stmts().size());
    auto nestedStmt = nestedBlock->stmts()[0].get();
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
    std::string_view returnKeywordString("return");
    std::string_view returnValueString("1");
    std::string_view nestedReturnKeywordString1("return");
    std::string_view nestedReturnValueString1("2");
    std::string_view nestedReturnKeywordString2("return");
    std::string_view nestedReturnValueString2("3");

    auto leftBrace = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_BRACE, "{", 1, 0, 0);
    auto returnKeyword = std::make_unique<sodium::Token>(sodium::TokenKind::KEYWORD, returnKeywordString.data(),
                                                         returnKeywordString.size(), 0, 0);
    auto returnValue = std::make_unique<sodium::Token>(sodium::TokenKind::NUMERIC_LITERAL, returnValueString.data(),
                                                       returnValueString.size(), 0, 0);
    auto semicolon1 = std::make_unique<sodium::Token>(sodium::TokenKind::SEMICOLON, ";", 1, 0, 0);
    auto nestedLeftBrace = std::make_unique<sodium::Token>(sodium::TokenKind::LEFT_BRACE, "{", 1, 0, 0);
    auto nestedReturnKeyword1 = std::make_unique<sodium::Token>(
        sodium::TokenKind::KEYWORD, nestedReturnKeywordString1.data(), nestedReturnKeywordString1.size(), 0, 0);
    auto nestedReturnValue1 = std::make_unique<sodium::Token>(
        sodium::TokenKind::NUMERIC_LITERAL, nestedReturnValueString1.data(), nestedReturnValueString1.size(), 0, 0);
    auto semicolon2 = std::make_unique<sodium::Token>(sodium::TokenKind::SEMICOLON, ";", 1, 0, 0);
    auto nestedReturnKeyword2 = std::make_unique<sodium::Token>(
        sodium::TokenKind::KEYWORD, nestedReturnKeywordString1.data(), nestedReturnKeywordString1.size(), 0, 0);
    auto nestedReturnValue2 = std::make_unique<sodium::Token>(
        sodium::TokenKind::NUMERIC_LITERAL, nestedReturnValueString1.data(), nestedReturnValueString1.size(), 0, 0);
    auto semicolon3 = std::make_unique<sodium::Token>(sodium::TokenKind::SEMICOLON, ";", 1, 0, 0);
    auto nestedRightBrace = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_BRACE, "}", 1, 0, 0);
    auto rightBrace = std::make_unique<sodium::Token>(sodium::TokenKind::RIGHT_BRACE, "}", 1, 0, 0);

    nestedRightBrace->next(std::move(rightBrace));
    semicolon3->next(std::move(nestedRightBrace));
    nestedReturnValue2->next(std::move(semicolon3));
    nestedReturnKeyword2->next(std::move(nestedReturnValue2));
    semicolon2->next(std::move(nestedReturnKeyword2));
    nestedReturnValue1->next(std::move(semicolon2));
    nestedReturnKeyword1->next(std::move(nestedReturnValue1));
    nestedLeftBrace->next(std::move(nestedReturnKeyword1));
    semicolon1->next(std::move(nestedLeftBrace));
    returnValue->next(std::move(semicolon1));
    returnKeyword->next(std::move(returnValue));
    leftBrace->next(std::move(returnKeyword));

    sodium::Parser parser(leftBrace.get());
    auto block(parser.parseBlock());

    ASSERT_EQ(2, block->stmts().size());

    auto stmt = block->stmts()[0].get();
    auto nestedBlock = static_cast<sodium::Block *>(block->stmts()[1].get());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, block->nodeKind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, block->stmtKind());

    EXPECT_EQ(sodium::StmtKind::RETURN, stmt->stmtKind());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, nestedBlock->nodeKind());
    EXPECT_EQ(sodium::StmtKind::BLOCK, nestedBlock->stmtKind());

    ASSERT_EQ(2, nestedBlock->stmts().size());
    auto nestedStmt1 = nestedBlock->stmts()[0].get();
    auto nestedStmt2 = nestedBlock->stmts()[1].get();

    EXPECT_EQ(sodium::StmtKind::RETURN, nestedStmt1->stmtKind());
    EXPECT_EQ(sodium::StmtKind::RETURN, nestedStmt2->stmtKind());
}

/*
    tests return statement:
        return 2;
*/
TEST(ParseStmtTest, ParserCorrectlyParsesReturnStmtWithSingleDigit) {
    std::string_view returnKeywordString("return");
    std::string_view returnValueString("2");

    auto returnKeyword = std::make_unique<sodium::Token>(sodium::TokenKind::KEYWORD, returnKeywordString.data(),
                                                         returnKeywordString.size(), 0, 0);
    auto returnValue = std::make_unique<sodium::Token>(sodium::TokenKind::NUMERIC_LITERAL, returnValueString.data(),
                                                       returnValueString.size(), 0, 0);
    auto semicolon = std::make_unique<sodium::Token>(sodium::TokenKind::SEMICOLON, ";", 1, 0, 0);

    returnValue->next(std::move(semicolon));
    returnKeyword->next(std::move(returnValue));

    sodium::Parser parser(returnKeyword.get());
    auto returnStmt(parser.parseReturnStmt());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, returnStmt->nodeKind());
    EXPECT_EQ(sodium::StmtKind::RETURN, returnStmt->stmtKind());

    sodium::NumericLiteralExpr *returnExpr = static_cast<sodium::NumericLiteralExpr *>(returnStmt->expr());

    EXPECT_EQ(2, returnExpr->value());
}

/*
    tests return statement:
        return 567;
*/
TEST(ParseStmtTest, ParserCorrectlyParsesReturnStmtWithMultipleDigit) {
    std::string_view returnKeywordString("return");
    std::string_view returnValueString("567");

    auto returnKeyword = std::make_unique<sodium::Token>(sodium::TokenKind::KEYWORD, returnKeywordString.data(),
                                                         returnKeywordString.size(), 0, 0);
    auto returnValue = std::make_unique<sodium::Token>(sodium::TokenKind::NUMERIC_LITERAL, returnValueString.data(),
                                                       returnValueString.size(), 0, 0);
    auto semicolon = std::make_unique<sodium::Token>(sodium::TokenKind::SEMICOLON, ";", 1, 0, 0);

    returnValue->next(std::move(semicolon));
    returnKeyword->next(std::move(returnValue));

    sodium::Parser parser(returnKeyword.get());
    auto returnStmt(parser.parseReturnStmt());

    EXPECT_EQ(sodium::ASTNodeKind::STMT, returnStmt->nodeKind());
    EXPECT_EQ(sodium::StmtKind::RETURN, returnStmt->stmtKind());

    sodium::NumericLiteralExpr *returnExpr = static_cast<sodium::NumericLiteralExpr *>(returnStmt->expr());

    EXPECT_EQ(567, returnExpr->value());
}
