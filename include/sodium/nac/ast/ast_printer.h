#ifndef SODIUM_NAC_AST_AST_PRINTER_H
#define SODIUM_NAC_AST_AST_PRINTER_H

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/ast_visitor.h"

namespace sodium {

class AST;
class SourceRange;

/// Used to print an AST to std::cout using the visitor pattern and ASTVisitor interface.
class ASTPrinter : public ASTVisitor {
public:
    /// Constructor for ASTPrinter.
    /// Constructs an ASTPrinter that uses an indentation width of 4 spaces.
    ASTPrinter();

    /// Constructor for ASTPrinter.
    /// \param spaces The  number of spaces of indentation used in the tree.
    ASTPrinter(int spaces);

    /// Print an AST.
    /// \param ast The AST to print.
    void print_ast(const AST &ast);

    /// Prints a source file ast node.
    /// \param source_file The source file node to be printed.
    void visit(const SourceFile &source_file) override;

    /// Prints an identifier.
    /// \param identifier The identifier to be printed.
    void visit(const Identifier &identifier) override;

    /// Prints a type.
    /// \param type The type to be printed.
    void visit(const Type &type) override;

    /// Print a function declaration.
    /// \param func_decl The function declaration to be printed.
    void visit(const FuncDecl &func_decl) override;

    /// Print a function signature.
    /// \param function_signature The function signature to be printed.
    void visit(const FuncSignature &func_signature) override;

    /// Prints a parameter list.
    /// \param parameter_list The parameter list to be printed.
    void visit(const ParameterList &parameter_list) override;

    /// Prints a numeric literal.
    /// \param integer_literal_expr The numeric literal to be printed.
    void visit(const IntegerLiteralExpr &integer_literal_expr) override;

    /// Prints a block.
    /// \param block The block to be printed.
    void visit(const Block &block) override;

    /// Prints a return statement.
    /// \param return_stmt The return statement to be printed.
    void visit(const ReturnStmt &return_stmt) override;

private:
    int indentation_spaces_;
    int indentation_level_;

    static void print_range(SourceRange range); // print the specified range
    void print_indentation() const;             // print the correct number of spaces of indentation

    void indent(); // increases the indentation level by 1
    void dedent(); // decreases the indentation level by 1
};

} // namespace sodium

#endif // SODIUM_NAC_AST_AST_PRINTER_H
