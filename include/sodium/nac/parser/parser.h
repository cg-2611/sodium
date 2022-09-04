#ifndef SODIUM_NAC_PARSER_PARSER_H
#define SODIUM_NAC_PARSER_PARSER_H

#include <memory>
#include <string_view>
#include <unordered_set>

#include "sodium/nac/ast/decl.h"
#include "sodium/nac/ast/expr.h"
#include "sodium/nac/ast/identifier.h"
#include "sodium/nac/ast/source_file.h"
#include "sodium/nac/ast/stmt.h"
#include "sodium/nac/ast/type.h"
#include "sodium/nac/lexer/lexer.h"
#include "sodium/nac/lexer/token.h"

namespace sodium {

class AST;
class Lexer;
class Token;

enum class TokenKind;

/**
 * @class Parser
 * @brief Used to parse a string of Sodium programming language tokens into an AST.
 *
 */
class Parser {
public:
    /**
     * @brief Construct a new Parser object.
     *
     * @param src An std::string_view that is the string to be parsed.
     */
    Parser(std::string_view src);

    /**
     * @brief Parses the string of Sodium programming language tokens.
     *
     * @return std::unique_ptr<AST> that represents the AST formed of the tokens in the string.
     */
    std::unique_ptr<AST> parse();

    /**
     * @brief Parses a source file, stops when an EOF token is encountered.
     *
     * @return std::unique_ptr<SourceFile> that is the generated source file AST node.
     */
    std::unique_ptr<SourceFile> parseSourceFile();

    /**
     * @brief Parses a declaration by dispatching to the relevant declaration subclass. To continue parsing after the
     *        declaration, the parser will need to be advanced to the next token.
     *
     * @return std::unique_ptr<Decl> that is the generated declaration AST node, or \c nullptr if a parsing error was
     *         encountered while parsing the declaration.
     */
    std::unique_ptr<Decl> parseDecl();

    /**
     * @brief Parses a function declaration. To continue parsing after the function declaration, the parser will need to
     *        be advanced to the next token.
     *
     * @return std::unique_ptr<FuncDecl> that is the generated function declaration AST node, or \c nullptr if there
     *         were parser errors encountered while parsing the function declaration.
     */
    std::unique_ptr<FuncDecl> parseFuncDecl();

    /**
     * @brief Parses a function signature. To continue parsing after the function signature, the parser will need to be
     *        advanced to the next token.
     *
     * @return std::unique_ptr<FunctionSignature> that is the generated function signature AST node, or \c nullptr if
     *         there were parser errors encountered while parsing the function signature.
     */
    std::unique_ptr<FunctionSignature> parseFunctionSignature();

    /**
     * @brief Parses a parameter list. To continue parsing after the parameter list, the parser will need to be advanced
     *        to the next token.
     *
     * @return std::unique_ptr<ParameterList> that is the generated parameter list AST node, or \c nullptr if there were
     *         parser errors encountered while parsing the parameter list.
     */
    std::unique_ptr<ParameterList> parseParameterList();

    /**
     * @brief Parses a function return type. To continue parsing after the return type, the parser will need to be
     *        advanced to the next token.
     *
     * @return std::unique_ptr<Type> that is the generated type AST node, or \c nullptr if there were
     *         parser errors encountered while parsing the return type.
     */
    std::unique_ptr<Type> parseReturnType();

    /**
     * @brief Parses a statement by dispatching to the relevant statement subclass. To continue parsing after the
     *         statement, the parser will need to be advanced to the next token.
     *
     * @return std::unique_ptr<Stmt> that is the generated statement AST node, or \c nullptr if there were parser errors
     *         encountered while parsing the statement.
     */
    std::unique_ptr<Stmt> parseStmt();

    /**
     * @brief Parses a block. There is no need to advance the parser to the next token after this function returns to
     *        continue parsing as this is handled in this method.
     *
     * @return std::unique_ptr<Block> that is the generated block AST node.
     */
    std::unique_ptr<Block> parseBlock();

    /**
     * @brief Parses a return statement. To continue parsing after the return statement, the parser will need to be
     *        advanced to the next token.
     *
     * @return std::unique_ptr<ReturnStmt> that is the generated return statement AST node, or \c nullptr if there were
     *         parser errors encountered while parsing the return statement.
     */
    std::unique_ptr<ReturnStmt> parseReturnStmt();

    /**
     * @brief Parses a expression by dispatching to the relevant expression subclass. To continue parsing after the
     *        expression, the parser will need to be advanced to the next token.
     *
     * @return std::unique_ptr<Expr> that is the generated expression AST node, or \c nullptr if there were
     *         parser errors encountered while parsing the expression.
     */
    std::unique_ptr<Expr> parseExpr();

    /**
     * @brief Parses a literal expression by dispatching to the relevant literal expression subclass. To continue
     *        parsing after the literal expression, the parser will need to be advanced to the next token.
     *
     * @return std::unique_ptr<LiteralExpr> that is the generated literal expression AST node, or \c nullptr if there
     *         were parser errors encountered while parsing the literal expression.
     */
    std::unique_ptr<LiteralExpr> parseLiteralExpr();

    /**
     * @brief Parses a numeric literal. To continue parsing after the numeric literal, the parser will need to be
     *        advanced to the next token.
     *
     * @return std::unique_ptr<NumericLiteralExpr> that is the generated numeric literal expression AST node, or \c
     *         nullptr if there were parser errors encountered while parsing the numeric literal.
     */
    std::unique_ptr<NumericLiteralExpr> parseNumericLiteralExpr();

    /**
     * @brief Parses an identifier. To continue parsing after the identifier, the parser will need to be advanced to the
     *        next token.
     *
     * @return std::unique_ptr<Identifier> that is the generated identifier AST node, or \c nullptr if there were parser
     *         errors encountered while parsing the identifier.
     */
    std::unique_ptr<Identifier> parseIdentifier();

    /**
     * @brief Parses a Type. To continue parsing after the type, the parser will need to be advanced to the next token.
     *
     * @return std::unique_ptr<Type> that is the generated type AST node, or \c nullptr if there were parser errors
     *         encountered while parsing the type.
     */
    std::unique_ptr<Type> parseType();

private:
    static const std::unordered_set<TokenKind> DECL_SYNCHRONIZING_TOKENS;
    static const std::unordered_set<TokenKind> STMT_SYNCHRONIZING_TOKENS;

    Lexer lexer_;
    Token token_;

    // advance the parser to the next token, token_ is set to the returned object of Lexer::getNextToken
    void advance() noexcept;

    // returns true if the kind of token_ matches the expected argument token kind
    bool match(TokenKind expectedTokenKind) const;

    // returns true if the kind of token_ matches the expected argument token kind
    // returns false an a failure to match and reports an error by passing message to errorExpected
    bool expect(TokenKind expectedTokenKind, std::string_view message) const;

    // reports the error described by message
    void errorExpected(std::string_view message) const;

    // used for error recovery, skips tokens until one of the synchronizing tokens is encountered
    void synchronize(const std::unordered_set<TokenKind> &synchronizingTokens);
};

} // namespace sodium

#endif // SODIUM_NAC_PARSER_PARSER_H
