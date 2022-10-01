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
    ASTPrinter(uint32_t spaces);

    /// Print an AST.
    /// \param ast The AST to print.
    void print_ast(const AST &ast);

    /// Prints a SourceFile AST node.
    /// \param source_file The SourceFile node to be printed.
    void visit(const SourceFile &source_file) override;

    /// Prints an Identifier.
    /// \param identifier The identifier to be printed.
    void visit(const Identifier &identifier) override;

    /// Prints a Type.
    /// \param type The Type to be printed.
    void visit(const Type &type) override;

    /// Print a FuncDecl.
    /// \param func_decl The FuncDecl to be printed.
    void visit(const FuncDecl &func_decl) override;

    /// Prints an IntegerLiteralExpr.
    /// \param integer_literal_expr The IntegerLiteralExpr to be printed.
    void visit(const IntegerLiteralExpr &integer_literal_expr) override;

    /// Prints a Block.
    /// \param block The Block to be printed.
    void visit(const Block &block) override;

    /// Prints a ReturnStmt.
    /// \param return_stmt The ReturnStmt to be printed.
    void visit(const ReturnStmt &return_stmt) override;

private:
    uint32_t indentation_spaces_;
    uint32_t indentation_level_;

    static void print_range(SourceRange range); // print the specified range
    void print_indentation() const;             // print the correct number of spaces of indentation

    void indent(); // increases the indentation level by 1
    void dedent(); // decreases the indentation level by 1
};

} // namespace sodium

#endif // SODIUM_NAC_AST_AST_PRINTER_H
