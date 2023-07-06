use crate::ast::decl::{Decl, FnDecl};
use crate::ast::expr::{Block, Expr, Literal, RetExpr};
use crate::ast::stmt::Stmt;
use crate::ast::ty::Type;
use crate::ast::{Identifier, SourceFile};

pub trait Visitor<'ast, T>: Sized {
    fn visit_source_file(&self, source_file: &'ast SourceFile) -> T;
    fn visit_decl(&self, decl: &'ast Decl) -> T;
    fn visit_fn_decl(&self, fn_decl: &'ast FnDecl) -> T;
    fn visit_ident(&self, ident: &'ast Identifier) -> T;
    fn visit_type(&self, ty: &'ast Type) -> T;
    fn visit_block(&self, block: &'ast Block) -> T;
    fn visit_stmt(&self, stmt: &'ast Stmt) -> T;
    fn visit_expr(&self, expr: &'ast Expr) -> T;
    fn visit_ret_expr(&self, ret_expr: &'ast RetExpr) -> T;
    fn visit_literal(&self, literal: &'ast Literal) -> T;
}

pub trait MutVisitor<'ast, T>: Sized {
    fn visit_source_file_mut(&mut self, source_file: &'ast SourceFile) -> T;
    fn visit_decl_mut(&mut self, decl: &'ast Decl) -> T;
    fn visit_fn_decl_mut(&mut self, fn_decl: &'ast FnDecl) -> T;
    fn visit_ident_mut(&mut self, ident: &'ast Identifier) -> T;
    fn visit_type_mut(&mut self, ty: &'ast Type) -> T;
    fn visit_block_mut(&mut self, block: &'ast Block) -> T;
    fn visit_stmt_mut(&mut self, stmt: &'ast Stmt) -> T;
    fn visit_expr_mut(&mut self, expr: &'ast Expr) -> T;
    fn visit_ret_expr_mut(&mut self, ret_expr: &'ast RetExpr) -> T;
    fn visit_literal_mut(&mut self, literal: &'ast Literal) -> T;
}
