#ifndef SODIUM_NAC_PARSER_PARSER_H
#define SODIUM_NAC_PARSER_PARSER_H

#include <exception>
#include <memory>
#include <string>

#include <iostream>

#include "sodium/nac/ast/ast.h"
#include "sodium/nac/ast/decl.h"
#include "sodium/nac/ast/expr.h"
#include "sodium/nac/ast/identifier.h"
#include "sodium/nac/ast/source_file.h"
#include "sodium/nac/ast/stmt.h"
#include "sodium/nac/ast/type.h"

namespace sodium {

class Token;

/**
 * @brief Used to parse a list of Sodium programming language tokens into an AST.
 *
 */
class Parser {
public:
    /**
     * @brief Construct a new Parser object.
     *
     * @param token A Token* that is the first token in the list of tokens to be parsed.
     */
    Parser(Token *token);

    /**
     * @brief Destroy the Parser object.
     *
     */
    ~Parser() = default;

    /**
     * @brief Parses the list of Sodium programming language tokens.
     *
     * @return std::unique_ptr<AST> that represents the AST formed of the tokens in the list.
     */
    std::unique_ptr<AST> parse();

    // each of the following methods parse the named rule from the grammar and creates and returns
    // an std::unique_ptr to the object created
    // the relevant grammar rules for each parse can be found commented before each method implementation
    // in the implementation files

    /**
     * @brief Parses a source file, stops when an EOF token is encountered.
     *
     * @return std::unique_ptr<SourceFile> that is the generated source file AST node.
     */
    std::unique_ptr<SourceFile> parseSourceFile();

    /**
     * @brief Parses a declaration by dispatching to the relevant declaration subclass.
     *
     * @return std::unique_ptr<Decl> that is the generated declaration AST node, or nullptr if an parsing error was
     *         encountered.
     */
    std::unique_ptr<Decl> parseDecl();

    /**
     * @brief Parses a function declaration.
     *
     * @return std::unique_ptr<FuncDecl> that is the generated function declaration AST node.
     */
    std::unique_ptr<FuncDecl> parseFuncDecl();

    /**
     * @brief Parses a function signature.
     *
     * @return std::unique_ptr<FunctionSignature> that is the generated function signature AST node.
     */
    std::unique_ptr<FunctionSignature> parseFunctionSignature();

    /**
     * @brief Parses a parameter list.
     *
     * @return std::unique_ptr<ParameterList> that is the generated parameter list AST node.
     */
    std::unique_ptr<ParameterList> parseParameterList();

    /**
     * @brief Parses a function return type.
     *
     * @return std::unique_ptr<Type> that is the generated type AST node.
     */
    std::unique_ptr<Type> parseReturnType();

    /**
     * @brief Parses a statement by dispatching to the relevant statement subclass.
     *
     * @return std::unique_ptr<Stmt> that is the generated statement AST node.
     */
    std::unique_ptr<Stmt> parseStmt();

    /**
     * @brief Parses a block.
     *
     * @return std::unique_ptr<Block> that is the generated block AST node.
     */
    std::unique_ptr<Block> parseBlock();

    /**
     * @brief Parses a return statement.
     *
     * @return std::unique_ptr<ReturnStmt> that is the generated return statement AST node.
     */
    std::unique_ptr<ReturnStmt> parseReturnStmt();

    /**
     * @brief Parses a expression by dispatching to the relevant expression subclass.
     *
     * @return std::unique_ptr<Expr> that is the generated expression AST node.
     */
    std::unique_ptr<Expr> parseExpr();

    /**
     * @brief Parses a literal expression by dispatching to the relevant literal expression subclass.
     *
     * @return std::unique_ptr<LiteralExpr> that is the generated literal expression AST node.
     */
    std::unique_ptr<LiteralExpr> parseLiteralExpr();

    /**
     * @brief Parses a numeric literal.
     *
     * @return std::unique_ptr<NumericLiteralExpr> that is the generated numeric literal expression AST node.
     */
    std::unique_ptr<NumericLiteralExpr> parseNumericLiteralExpr();

    /**
     * @brief Parses an identifier.
     *
     * @return std::unique_ptr<Identifier> that is the generated identifier AST node.
     */
    std::unique_ptr<Identifier> parseIdentifier();

    /**
     * @brief Parses a Type.
     *
     * @return std::unique_ptr<Type> that is the generated type AST node.
     */
    std::unique_ptr<Type> parseType();

    /**
     * @brief Check of a statement is terminated.
     *
     * @return true if the current token is an EOL token or a } token,
     * @return false otherwise.
     */
    bool isStmtTerminated();

private:
    Token *token_;

    void advance() noexcept;    // advance the parser to the next token, token_ is changed
    void skipExcessEOLTokens(); // skip over any insignificant EOL tokens

    Token *nextToken() const noexcept; // returns a pointer to the token after token_, token_ is not changed

    void synchronize();
};

} // namespace sodium

#endif // SODIUM_NAC_PARSER_PARSER_H

// #ifndef SODIUM_NAC_PARSER_PARSER_H
// #define SODIUM_NAC_PARSER_PARSER_H

// #include <exception>
// #include <memory>
// #include <string>

// #include <iostream>

// #include "sodium/nac/ast/ast.h"
// #include "sodium/nac/ast/decl.h"
// #include "sodium/nac/ast/expr.h"
// #include "sodium/nac/ast/identifier.h"
// #include "sodium/nac/ast/parameter_list.h"
// #include "sodium/nac/ast/source_file.h"
// #include "sodium/nac/ast/stmt.h"
// #include "sodium/nac/ast/type.h"

// namespace sodium {

// class Token;

// /**
//  * @brief Used to parse a list of Sodium programming language tokens into an AST.
//  *
//  */
// class Parser {
// public:
//     /**
//      * @brief Construct a new Parser object.
//      *
//      * @param token A Token* that is the first token in the list of tokens to be parsed.
//      */
//     Parser(Token *token);

//     /**
//      * @brief Destroy the Parser object.
//      *
//      */
//     ~Parser() = default;

//     /**
//      * @brief Parses the list of Sodium programming language tokens.
//      *
//      * @return std::unique_ptr<AST> that represents the AST formed of the tokens in the list.
//      */
//     std::unique_ptr<AST> parse();

//     // each of the following methods parse the named rule from the grammar and creates and returns
//     // an std::unique_ptr to the object created
//     // the relevant grammar rules for each parse can be found commented before each method implementation
//     // in the implementation files

//     // implementation in parser.cpp

//     /**
//      * @brief Parses a source file, stops when an EOF token is encountered.
//      *
//      * @return std::unique_ptr<SourceFile> that is the generated source file AST node.
//      */
//     std::unique_ptr<SourceFile> parseSourceFile();

//     /**
//      * @brief Parses an identifier.
//      *
//      * @return std::unique_ptr<Identifier> that is the generated identifier AST node.
//      */
//     std::unique_ptr<Identifier> parseIdentifier();

//     /**
//      * @brief Parses a Type.
//      *
//      * @return std::unique_ptr<Type> that is the generated type AST node.
//      */
//     std::unique_ptr<Type> parseType();

//     // implementation in parse_decl.cpp

//     /**
//      * @brief Parses a declaration by dispatching to the relevant declaration subclass.
//      *
//      * @return std::unique_ptr<Decl> that is the generated declaration AST node, or nullptr if an parsing error was
//      *         encountered.
//      */
//     std::unique_ptr<Decl> parseDecl();

//     /**
//      * @brief Parses a function declaration.
//      *
//      * @return std::unique_ptr<FuncDecl> that is the generated function declaration AST node.
//      */
//     std::unique_ptr<FuncDecl> parseFuncDecl();

//     /**
//      * @brief Parses a function signature.
//      *
//      * @return std::unique_ptr<FunctionSignature> that is the generated function signature AST node.
//      */
//     std::unique_ptr<FunctionSignature> parseFunctionSignature();

//     /**
//      * @brief Parses a parameter list.
//      *
//      * @return std::unique_ptr<ParameterList> that is the generated parameter list AST node.
//      */
//     std::unique_ptr<ParameterList> parseParameterList();

//     /**
//      * @brief Parses a parameter.
//      *
//      * @return std::unique_ptr<Parameter> that is the generated parameter AST node.
//      */
//     std::unique_ptr<Parameter> parseParameter();

//     /**
//      * @brief Parses a function return type.
//      *
//      * @return std::unique_ptr<Type> that is the generated type AST node.
//      */
//     std::unique_ptr<Type> parseReturnType();

//     // implementation in parse_expr.cpp

//     /**
//      * @brief Parses a expression by dispatching to the relevant expression subclass.
//      *
//      * @return std::unique_ptr<Expr> that is the generated expression AST node.
//      */
//     std::unique_ptr<Expr> parseExpr();

//     /**
//      * @brief Parses a literal expression by dispatching to the relevant literal expression subclass.
//      *
//      * @return std::unique_ptr<LiteralExpr> that is the generated literal expression AST node.
//      */
//     std::unique_ptr<LiteralExpr> parseLiteralExpr();

//     /**
//      * @brief Parses a numeric literal.
//      *
//      * @return std::unique_ptr<NumericLiteralExpr> that is the generated numeric literal expression AST node.
//      */
//     std::unique_ptr<NumericLiteralExpr> parseNumericLiteralExpr();

//     // implementation in parse_stmt.cpp

//     /**
//      * @brief Parses a statement by dispatching to the relevant statement subclass.
//      *
//      * @return std::unique_ptr<Stmt> that is the generated statement AST node.
//      */
//     std::unique_ptr<Stmt> parseStmt();

//     /**
//      * @brief Parses a block.
//      *
//      * @return std::unique_ptr<Block> that is the generated block AST node.
//      */
//     std::unique_ptr<Block> parseBlock();

//     /**
//      * @brief Parses a return statement.
//      *
//      * @return std::unique_ptr<ReturnStmt> that is the generated return statement AST node.
//      */
//     std::unique_ptr<ReturnStmt> parseReturnStmt();

//     /**
//      * @brief Check of a statement is terminated.
//      *
//      * @return true if the current token is an EOL token or a } token,
//      * @return false otherwise.
//      */
//     bool isStmtTerminated();

// private:
//     Token *token_;

//     void advance() noexcept;    // advance the parser to the next token, token_ is changed
//     void skipExcessEOLTokens(); // skip over any insignificant EOL tokens

//     Token *nextToken() const noexcept; // returns a pointer to the token after token_, token_ is not changed

//     void synchronize();
// };

// class InternalParserException : public std::exception {
// public:
//     InternalParserException(const std::string &message) : message_(message) {}
//     ~InternalParserException() = default;

//     const char *what() const noexcept override {
//         return message_.c_str();
//     }

// private:
//     std::string message_;
// };

// } // namespace sodium

// #endif // SODIUM_NAC_PARSER_PARSER_H
