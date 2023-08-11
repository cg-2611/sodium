use ir::{Block, Expr, ExprKind, Literal, LiteralKind, RetExpr};

use crate::lower::ASTLower;
use crate::SemaResult;

impl<'ctx, 'ast> ASTLower<'ctx> {
    pub fn lower_block(&self, block: &'ast ast::Block) -> SemaResult<'ctx, Box<Block>> {
        let mut stmts = Vec::new();

        for ast_stmt in &block.stmts {
            let stmt = self.lower_stmt(ast_stmt)?;
            stmts.push(stmt);
        }

        if stmts.is_empty() {
            return Err(self
                .context
                .session()
                .create_ranged_error(String::from("empty block"), block.range));
        }

        let block_ty = stmts.last().unwrap().ty.clone();

        Ok(Box::new(Block::new(stmts, block_ty, block.range)))
    }

    pub fn lower_expr(&self, expr: &'ast ast::Expr) -> SemaResult<'ctx, Box<Expr>> {
        match &expr.kind {
            ast::ExprKind::Block(ast_block) => {
                let block = self.lower_block(ast_block)?;
                let block_ty = block.ty.clone();
                Ok(Box::new(Expr::new(
                    ExprKind::Block(block),
                    block_ty,
                    ast_block.range,
                )))
            }
            ast::ExprKind::Ret(ast_ret_expr) => {
                let ret_expr = self.lower_ret_expr(ast_ret_expr)?;
                let ret_expr_ty = ret_expr.ty.clone();
                Ok(Box::new(Expr::new(
                    ExprKind::Ret(ret_expr),
                    ret_expr_ty,
                    ast_ret_expr.range,
                )))
            }
            ast::ExprKind::Literal(ast_literal) => {
                let integer_literal = self.lower_literal(ast_literal)?;
                let literal_ty = integer_literal.ty.clone();
                Ok(Box::new(Expr::new(
                    ExprKind::Literal(integer_literal),
                    literal_ty,
                    ast_literal.range,
                )))
            }
        }
    }

    pub fn lower_ret_expr(&self, ret_expr: &'ast ast::RetExpr) -> SemaResult<'ctx, Box<RetExpr>> {
        let expr = self.lower_expr(&ret_expr.expr)?;
        let expr_ty = expr.ty.clone();
        Ok(Box::new(RetExpr::new(expr, expr_ty, ret_expr.range)))
    }

    pub fn lower_literal(&self, literal: &'ast ast::Literal) -> SemaResult<'ctx, Literal> {
        match literal.kind {
            ast::LiteralKind::Integer(x) => Ok(Literal::new(
                LiteralKind::Integer(x),
                self.context.type_context().i32(),
                literal.range,
            )),
        }
    }
}
