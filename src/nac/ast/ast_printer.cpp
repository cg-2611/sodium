#include "sodium/nac/ast/ast_printer.h"

#include <iostream>

#include "sodium/nac/ast/ast.h"
#include "sodium/nac/ast/ast_visitor.h"
#include "sodium/nac/ast/decl.h"
#include "sodium/nac/ast/expr.h"
#include "sodium/nac/ast/identifier.h"
#include "sodium/nac/ast/source_file.h"
#include "sodium/nac/ast/stmt.h"
#include "sodium/nac/ast/type.h"
#include "sodium/nac/basic/source_range.h"

namespace sodium {

ASTPrinter::ASTPrinter() : ASTPrinter(4) {}

ASTPrinter::ASTPrinter(uint32_t spaces) : indentation_spaces_(spaces), indentation_level_(0) {}

void ASTPrinter::print_ast(const AST &ast) {
    ast.root()->accept(*this);
}

void ASTPrinter::visit(const SourceFile &source_file) {
    std::cout << "source file:";
    print_range(source_file.range());

    indent();

    // visit the declarations in the source file
    for (auto &&decl : source_file.decls()) {
        decl->accept(*this);
    }

    dedent();
}

void ASTPrinter::visit(const FuncDecl &func_decl) {
    print_indentation();
    std::cout << "func decl:";
    print_range(func_decl.range());

    indent();

    func_decl.name()->accept(*this);
    func_decl.return_type()->accept(*this);
    func_decl.body()->accept(*this);

    dedent();
}

void ASTPrinter::visit(const Block &block) {
    print_indentation();
    std::cout << "block:";
    print_range(block.range());

    indent();

    // visit the statements in the block
    for (auto &&stmt : block.stmts()) {
        stmt->accept(*this);
    }

    dedent();
}

void ASTPrinter::visit(const ReturnStmt &return_stmt) {
    print_indentation();
    std::cout << "return:";
    print_range(return_stmt.range());

    indent();

    // visit the expression of the return statement
    return_stmt.expr()->accept(*this);

    dedent();
}

void ASTPrinter::visit(const IntegerLiteralExpr &integer_literal_expr) {
    print_indentation();
    std::cout << "value: " << integer_literal_expr.value();
    print_range(integer_literal_expr.range());
}

void ASTPrinter::visit(const Identifier &identifier) {
    print_indentation();
    std::cout << "identifier: " << identifier.value();
    print_range(identifier.range());
}

void ASTPrinter::visit(const Type &type) {
    print_indentation();
    std::cout << "return type: " << type.name();
    print_range(type.range());
}

void ASTPrinter::print_indentation() const {
    auto spaces = indentation_spaces_ * indentation_level_;
    for (size_t i = 0; i < spaces; ++i) {
        std::cout << " ";
    }
}

void ASTPrinter::print_range(SourceRange range) {
    std::cout << " (" << range.start().to_string() << " - " << range.end().to_string() << ")\n";
}

void ASTPrinter::indent() {
    ++indentation_level_;
}

void ASTPrinter::dedent() {
    --indentation_level_;
}

} // namespace sodium
