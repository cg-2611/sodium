use ir::{Stmt, StmtKind};

use crate::lower::ASTLower;
use crate::SemaResult;

impl<'ctx, 'ast> ASTLower<'ctx> {
    pub fn lower_stmt(&self, stmt: &'ast ast::Stmt) -> SemaResult<'ctx, Stmt> {
        self.lower_expr_stmt(stmt)
    }

    pub fn lower_expr_stmt(&self, stmt: &'ast ast::Stmt) -> SemaResult<'ctx, Stmt> {
        let expr = match &stmt.kind {
            ast::StmtKind::ExprStmt(ast_expr) => self.lower_expr(ast_expr)?,
        };

        let expr_stmt_ty = expr.ty.clone();

        Ok(Stmt::new(
            StmtKind::ExprStmt(expr),
            expr_stmt_ty,
            stmt.range,
        ))
    }
}
