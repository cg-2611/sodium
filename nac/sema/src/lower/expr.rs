use crate::ir::{Block, Expr, ExprKind, Literal, LiteralKind, RetExpr, Stmt};
use crate::lower::ASTLower;
use crate::ty::Type;

impl<'cx, 'ast> ASTLower {
    pub fn lower_block(
        &self,
        block: &'ast ast::Block,
        stmts: Vec<Stmt<'cx>>,
        ty: Type<'cx>,
    ) -> Box<Block<'cx>> {
        Box::new(Block::new(stmts, ty, block.range))
    }

    pub fn lower_expr(
        &self,
        expr: &'ast ast::Expr,
        kind: ExprKind<'cx>,
        ty: Type<'cx>,
    ) -> Box<Expr<'cx>> {
        Box::new(Expr::new(kind, ty, expr.range))
    }

    pub fn lower_ret_expr(
        &self,
        ret_expr: &'ast ast::RetExpr,
        expr: Box<Expr<'cx>>,
        ty: Type<'cx>,
    ) -> Box<RetExpr<'cx>> {
        Box::new(RetExpr::new(expr, ty, ret_expr.range))
    }

    pub fn lower_literal(
        &self,
        literal: &'ast ast::Literal,
        kind: LiteralKind,
        ty: Type<'cx>,
    ) -> Literal<'cx> {
        Literal::new(kind, ty, literal.range)
    }
}
