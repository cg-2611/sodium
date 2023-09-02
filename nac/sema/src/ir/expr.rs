use range::Range;

use crate::ir::Stmt;
use crate::ty::Type;

pub enum ExprKind<'cx> {
    Block(Block<'cx>),
    Literal(Literal<'cx>),
    Unary(UnaryExpr<'cx>),
    Binary(BinaryExpr<'cx>),
    Ret(RetExpr<'cx>),
}

pub struct Expr<'cx> {
    pub kind: ExprKind<'cx>,
    pub ty: Type<'cx>,
    pub range: Range,
}

impl<'cx> Expr<'cx> {
    pub fn new(kind: ExprKind<'cx>, ty: Type<'cx>, range: Range) -> Self {
        Self { kind, ty, range }
    }
}

pub struct Block<'cx> {
    pub stmts: Vec<Stmt<'cx>>,
    pub ty: Type<'cx>,
    pub range: Range,
}

impl<'cx> Block<'cx> {
    pub fn new(stmts: Vec<Stmt<'cx>>, ty: Type<'cx>, range: Range) -> Self {
        Self { stmts, ty, range }
    }
}

#[derive(Debug, PartialEq)]
pub enum LiteralKind {
    Integer(i32),
}

pub struct Literal<'cx> {
    pub kind: LiteralKind,
    pub ty: Type<'cx>,
    pub range: Range,
}

impl<'cx> Literal<'cx> {
    pub fn new(kind: LiteralKind, ty: Type<'cx>, range: Range) -> Self {
        Self { kind, ty, range }
    }
}

pub enum UnaryOperator {
    Negate,
}

pub struct UnaryExpr<'cx> {
    pub operator: UnaryOperator,
    pub expr: Box<Expr<'cx>>,
    pub ty: Type<'cx>,
    pub range: Range,
}

impl<'cx> UnaryExpr<'cx> {
    pub fn new(operator: UnaryOperator, expr: Box<Expr<'cx>>, ty: Type<'cx>, range: Range) -> Self {
        Self {
            operator,
            expr,
            ty,
            range,
        }
    }
}

pub enum BinaryOperator {
    Add,
    Subtract,
    Multiply,
    Divide,
}

pub struct BinaryExpr<'cx> {
    pub operator: BinaryOperator,
    pub lhs: Box<Expr<'cx>>,
    pub rhs: Box<Expr<'cx>>,
    pub ty: Type<'cx>,
    pub range: Range,
}

impl<'cx> BinaryExpr<'cx> {
    pub fn new(
        operator: BinaryOperator,
        lhs: Box<Expr<'cx>>,
        rhs: Box<Expr<'cx>>,
        ty: Type<'cx>,
        range: Range,
    ) -> Self {
        Self {
            operator,
            lhs,
            rhs,
            ty,
            range,
        }
    }
}

pub struct RetExpr<'cx> {
    pub expr: Box<Expr<'cx>>,
    pub ty: Type<'cx>,
    pub range: Range,
}

impl<'cx> RetExpr<'cx> {
    pub fn new(expr: Box<Expr<'cx>>, ty: Type<'cx>, range: Range) -> Self {
        Self { expr, ty, range }
    }
}
