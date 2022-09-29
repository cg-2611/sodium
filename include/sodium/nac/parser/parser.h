#ifndef SODIUM_NAC_PARSER_PARSER_H
#define SODIUM_NAC_PARSER_PARSER_H

#include <memory>

#include "sodium/nac/ast/decl.h"
#include "sodium/nac/ast/expr.h"
#include "sodium/nac/ast/identifier.h"
#include "sodium/nac/ast/source_file.h"
#include "sodium/nac/ast/stmt.h"
#include "sodium/nac/ast/type.h"
#include "sodium/nac/token/token.h"
#include "sodium/nac/token/token_cursor.h"

namespace sodium {

class AST;
class DiagnosticEngine;
class TokenBuffer;

enum class ParserErrorKind;
enum class TokenKind;

/// Used to parse Sodium programming language tokens into an AST.
class Parser {
public:
    /// Constructor for Parser.
    /// \param token_buffer The tokens to be parsed.
    Parser(const TokenBuffer &token_buffer, DiagnosticEngine &diagnostic_engine);

    /// Parses Sodium programming language tokens into an AST.
    /// \return The AST formed of the parsed tokens.
    AST parse();

    /// Parses the declarations of a source file, stops when an EOF token is encountered.
    /// \return The parsed source file AST node.
    std::unique_ptr<SourceFile> parse_source_file();

    /// Parses an identifier.
    /// \return The parsed identifier AST node, or \c nullptr if there was an error while parsing the identifier.
    std::unique_ptr<Identifier> parse_identifier();

    /// Parses a type.
    /// \return The parsed type AST node, or \c nullptr if there was an error while parsing the type.
    std::unique_ptr<Type> parse_type();

    /// Parses a declaration by dispatching to the relevant declaration subclass.
    /// \return The parsed declaration AST node, or \c nullptr if there was an error while parsing the declaration.
    std::unique_ptr<Decl> parse_decl();

    /// Parses a function declaration.
    /// \return The parsed function declaration AST node, or \c nullptr if there was an error while parsing the function
    /// declaration.
    std::unique_ptr<FuncDecl> parse_func_decl();

    /// Parses an function return type.
    /// \return The parsed function return type AST node, or \c nullptr if there was an error while parsing the function
    /// return type.
    std::unique_ptr<Type> parse_return_type();

    /// Parses a expression by dispatching to the relevant expression subclass.
    /// \return The parsed expression AST node, or \c nullptr if there was an error while parsing the expression.
    std::unique_ptr<Expr> parse_expr();

    /// Parses an integer literal.
    /// \return The parsed integer literal AST node, or \c nullptr if there was an error while parsing the integer
    /// literal.
    std::unique_ptr<IntegerLiteralExpr> parse_integer_literal_expr();

    /// Parses a statement by dispatching to the relevant statement subclass.
    /// \return The parsed statement AST node, or \c nullptr if there was an error while parsing the statement.
    std::unique_ptr<Stmt> parse_stmt();

    /// Parses an block.
    /// \return The parsed block AST node, or \c nullptr if there was an error while parsing the block.
    std::unique_ptr<Block> parse_block();

    /// Parses an return statement.
    /// \return The parsed return statement AST node, or \c nullptr if there was an error while parsing the return
    /// statement.
    std::unique_ptr<ReturnStmt> parse_return_stmt();

private:
    DiagnosticEngine &diagnostic_engine_;

    TokenCursor token_cursor_;
    Token token_;

    // advance the parser to the next token from the TokenCursor
    void advance();

    // returns true if the kind of token_ matches the expected token kind
    // returns false otherwise
    [[nodiscard]] bool match(TokenKind expected) const;

    // returns true if the kind of token_ matches the expected token kind, the parser does advance
    // returns false on a failure to match and diagnoses the error indicated by the kind parameter, the parser does not
    // advance
    bool expect(TokenKind expected, ParserErrorKind kind);

    // diagnoses the error indicated by the kind parameter
    void error_expected(ParserErrorKind kind) const;

    // used for error recovery when failing to parse a declaration
    // parsing is ready to resume after this function returns
    void synchronize_decl();

    // used for error recovery when failing to parse a statement
    // parsing is ready to resume after this function returns
    void synchronize_stmt();
};

} // namespace sodium

#endif // SODIUM_NAC_PARSER_PARSER_H
