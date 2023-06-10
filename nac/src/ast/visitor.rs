use crate::ast::decl::{Decl, DeclKind, FnDecl};
use crate::ast::expr::{Block, Expr, ExprKind, Literal, LiteralKind, RetExpr};
use crate::ast::stmt::{Stmt, StmtKind};
use crate::ast::ty::Type;
use crate::ast::{Identifier, SourceFile};

pub trait MutVisitor<'ast>: Sized {
    fn visit_source_file(&mut self, source_file: &'ast SourceFile) {
        walk_source_file(self, source_file);
    }

    fn visit_decl(&mut self, decl: &'ast Decl) {
        walk_decl(self, decl);
    }

    fn visit_fn_decl(&mut self, fn_decl: &'ast FnDecl) {
        walk_fn_decl(self, fn_decl);
    }

    fn visit_ident(&mut self, ident: &'ast Identifier) {
        walk_ident(self, ident);
    }

    fn visit_type(&mut self, ty: &'ast Type) {
        walk_type(self, ty);
    }

    fn visit_block(&mut self, block: &'ast Block) {
        walk_block(self, block);
    }

    fn visit_stmt(&mut self, stmt: &'ast Stmt) {
        walk_stmt(self, stmt);
    }

    fn visit_expr(&mut self, expr: &'ast Expr) {
        walk_expr(self, expr);
    }

    fn visit_ret_expr(&mut self, ret_expr: &'ast RetExpr) {
        walk_ret_expr(self, ret_expr);
    }

    fn visit_literal(&mut self, literal: &'ast Literal) {
        walk_literal(self, literal);
    }
}
pub fn walk_source_file<'ast, V: MutVisitor<'ast>>(visitor: &mut V, source_file: &'ast SourceFile) {
    for decl in &source_file.decls {
        visitor.visit_decl(decl);
    }
}

pub fn walk_decl<'ast, V: MutVisitor<'ast>>(visitor: &mut V, decl: &'ast Decl) {
    match &decl.kind {
        DeclKind::Fn(fn_decl) => visitor.visit_fn_decl(fn_decl),
    }
}

pub fn walk_fn_decl<'ast, V: MutVisitor<'ast>>(visitor: &mut V, fn_decl: &'ast FnDecl) {
    visitor.visit_ident(&fn_decl.ident);
    visitor.visit_type(&fn_decl.ret_type);
    visitor.visit_block(&fn_decl.body);
}

pub fn walk_ident<'ast, V: MutVisitor<'ast>>(visitor: &mut V, ident: &'ast Identifier) {
    let _ = visitor;
    let _ = ident;
}

pub fn walk_type<'ast, V: MutVisitor<'ast>>(visitor: &mut V, ty: &'ast Type) {
    visitor.visit_ident(&ty.ident);
}

pub fn walk_block<'ast, V: MutVisitor<'ast>>(visitor: &mut V, block: &'ast Block) {
    for stmt in &block.stmts {
        visitor.visit_stmt(stmt);
    }
}

pub fn walk_stmt<'ast, V: MutVisitor<'ast>>(visitor: &mut V, stmt: &'ast Stmt) {
    match &stmt.kind {
        StmtKind::ExprStmt(expr) => visitor.visit_expr(expr),
    }
}

pub fn walk_expr<'ast, V: MutVisitor<'ast>>(visitor: &mut V, expr: &'ast Expr) {
    match &expr.kind {
        ExprKind::Block(block) => visitor.visit_block(block),
        ExprKind::Literal(literal) => visitor.visit_literal(literal),
        ExprKind::Ret(ret_expr) => visitor.visit_ret_expr(ret_expr),
    }
}

pub fn walk_ret_expr<'ast, V: MutVisitor<'ast>>(visitor: &mut V, ret_expr: &'ast RetExpr) {
    visitor.visit_expr(&ret_expr.expr);
}

pub fn walk_literal<'ast, V: MutVisitor<'ast>>(visitor: &mut V, literal: &'ast Literal) {
    let _ = visitor;
    match &literal.kind {
        LiteralKind::Integer(x) => {
            let _ = x;
        }
    }
}
