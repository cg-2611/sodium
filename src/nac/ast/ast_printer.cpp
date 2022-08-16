#include "sodium/nac/ast/ast_printer.h"

#include <iostream>

#include "sodium/nac/ast/ast.h"
#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/decl.h"
#include "sodium/nac/ast/expr.h"
#include "sodium/nac/ast/identifier.h"
#include "sodium/nac/ast/parameter_list.h"
#include "sodium/nac/ast/source_file.h"
#include "sodium/nac/ast/stmt.h"
#include "sodium/nac/ast/type.h"

namespace sodium {

ASTPrinter::ASTPrinter(int spaces) : indentationSpaces_(spaces), indentationLevel_(0) {}

void ASTPrinter::printAST(const AST *ast) {
    ASTNode *root = ast->root();
    switch (root->nodeKind()) {
        case ASTNodeKind::DECL: visit(static_cast<const Decl *>(root)); break;
        case ASTNodeKind::EXPR: visit(static_cast<const Expr *>(root)); break;
        case ASTNodeKind::IDENTIFIER: visit(static_cast<const Identifier *>(root)); break;
        case ASTNodeKind::PARAMETER: visit(static_cast<const Parameter *>(root)); break;
        case ASTNodeKind::PARAMETER_LIST: visit(static_cast<const ParameterList *>(root)); break;
        case ASTNodeKind::SOURCE_FILE: visit(static_cast<const SourceFile *>(root)); break;
        case ASTNodeKind::SIGNATURE: visit(static_cast<const Signature *>(root)); break;
        case ASTNodeKind::STMT: visit(static_cast<const Stmt *>(root)); break;
        case ASTNodeKind::TYPE: visit(static_cast<const Type *>(root)); break;
        default: std::cout << "unknown ast node kind\n"; break;
    }
}

void ASTPrinter::visit(const Decl *decl) {
    switch (decl->declKind()) {
        case DeclKind::FUNCTION: visit(static_cast<const FuncDecl *>(decl)); break;
        default: std::cout << "unknown declaration kind\n"; break;
    }
}

void ASTPrinter::visit(const Signature *signature) {
    printIndentation();
    std::cout << "signature:\n";

    indent();

    visit(signature->name());
    visit(signature->parameters());
    visit(signature->returnType());

    dedent();
}

void ASTPrinter::visit(const FuncDecl *funcDecl) {
    printIndentation();
    std::cout << "func decl:\n";

    indent();

    visit(funcDecl->signature());
    visit(funcDecl->body());

    dedent();
}

void ASTPrinter::visit(const Expr *expr) {
    switch (expr->exprKind()) {
        case ExprKind::LITERAL: visit(static_cast<const LiteralExpr *>(expr)); break;
        default: std::cout << "unknown expression kind\n"; break;
    }
}
void ASTPrinter::visit(const LiteralExpr *literalExpr) {
    switch (literalExpr->literalKind()) {
        case LiteralKind::NUMERIC_LITERAL: visit(static_cast<const NumericLiteralExpr *>(literalExpr)); break;
        default: std::cout << "unknown literal expression kind\n"; break;
    }
}

void ASTPrinter::visit(const NumericLiteralExpr *numericLiteralExpr) {
    printIndentation();
    std::cout << "value: " << numericLiteralExpr->value() << '\n';
}

void ASTPrinter::visit(const Identifier *identifier) {
    printIndentation();
    std::cout << "identifier: " << identifier->value() << '\n';
}

void ASTPrinter::visit(const Parameter *parameter) {
    printIndentation();
    std::cout << "parameter:\n";

    indent();

    visit(parameter->identifier());

    dedent();
}

void ASTPrinter::visit(const ParameterList *parameterList) {
    printIndentation();
    std::cout << "parameters:\n";

    indent();

    // visit the parameters in the parameter list
    for (const std::unique_ptr<Parameter> &parameter : parameterList->parameters()) {
        visit(parameter.get());
    }

    dedent();
}

void ASTPrinter::visit(const SourceFile *sourceFile) {
    std::cout << "source file:\n";

    indent();

    // visit the declarations in the source file
    for (const std::unique_ptr<Decl> &decl : sourceFile->decls()) {
        visit(decl.get());
    }

    dedent();
}

void ASTPrinter::visit(const Stmt *stmt) {
    switch (stmt->stmtKind()) {
        case StmtKind::BLOCK: visit(static_cast<const Block *>(stmt)); break;
        case StmtKind::RETURN: visit(static_cast<const ReturnStmt *>(stmt)); break;
        default: std::cout << "unknown statement kind\n"; break;
    }
}

void ASTPrinter::visit(const Block *block) {
    printIndentation();
    std::cout << "block:\n";

    indent();

    // visit the statements in the block
    for (const std::unique_ptr<Stmt> &stmt : block->stmts()) {
        visit(stmt.get());
    }

    dedent();
}

void ASTPrinter::visit(const ReturnStmt *returnStmt) {
    printIndentation();
    std::cout << "return:\n";

    indent();

    visit(returnStmt->expr());

    dedent();
}

void ASTPrinter::visit(const Type *type) {
    printIndentation();
    std::cout << "return type: " << type->name() << '\n';
}

void ASTPrinter::printIndentation() {
    int spaces = indentationSpaces_ * indentationLevel_;
    for (int i = 0; i < spaces; ++i) {
        std::cout << " ";
    }
}

void ASTPrinter::indent() {
    ++indentationLevel_;
}

void ASTPrinter::dedent() {
    --indentationLevel_;
}

} // namespace sodium
