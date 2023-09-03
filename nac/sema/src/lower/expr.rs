use crate::ir::expr::{BinaryExpr, BinaryOperator, UnaryExpr, UnaryOperator};
use crate::ir::{Block, Expr, ExprKind, Literal, LiteralKind, RetExpr, Stmt};
use crate::lower::ASTLower;
use crate::ty::Type;

impl<'cx, 'ast> ASTLower {
    pub fn lower_block(
        &self,
        block: &'ast ast::Block,
        stmts: Vec<Stmt<'cx>>,
        ty: Type<'cx>,
    ) -> Block<'cx> {
        Block::new(stmts, ty, block.range)
    }

    pub fn lower_expr(
        &self,
        expr: &'ast ast::Expr,
        kind: ExprKind<'cx>,
        ty: Type<'cx>,
    ) -> Expr<'cx> {
        Expr::new(kind, ty, expr.range)
    }

    pub fn lower_literal(
        &self,
        literal: &'ast ast::Literal,
        kind: LiteralKind,
        ty: Type<'cx>,
    ) -> Literal<'cx> {
        Literal::new(kind, ty, literal.range)
    }

    pub fn lower_unary_expr(
        &self,
        unary_expr: &'ast ast::UnaryExpr,
        operator: UnaryOperator,
        expr: Box<Expr<'cx>>,
        ty: Type<'cx>,
    ) -> UnaryExpr<'cx> {
        UnaryExpr::new(operator, expr, ty, unary_expr.range)
    }

    pub fn lower_binary_expr(
        &self,
        binary_expr: &'ast ast::BinaryExpr,
        operator: BinaryOperator,
        lhs: Box<Expr<'cx>>,
        rhs: Box<Expr<'cx>>,
        ty: Type<'cx>,
    ) -> BinaryExpr<'cx> {
        BinaryExpr::new(operator, lhs, rhs, ty, binary_expr.range)
    }

    pub fn lower_ret_expr(
        &self,
        ret_expr: &'ast ast::RetExpr,
        expr: Box<Expr<'cx>>,
        ty: Type<'cx>,
    ) -> RetExpr<'cx> {
        RetExpr::new(expr, ty, ret_expr.range)
    }
}
