use range::Range;

use crate::Stmt;

pub enum ExprKind {
    Block(Block),
    Literal(Literal),
    Binary(BinaryExpr),
    Unary(UnaryExpr),
    Ret(RetExpr),
}

pub struct Expr {
    pub kind: ExprKind,
    pub range: Range,
}

impl Expr {
    pub fn new(kind: ExprKind, range: Range) -> Self {
        Self { kind, range }
    }
}

pub struct Block {
    pub stmts: Vec<Stmt>,
    pub range: Range,
}

impl Block {
    pub fn new(stmts: Vec<Stmt>, range: Range) -> Self {
        Self { stmts, range }
    }
}

#[derive(Debug, PartialEq)]
pub enum LiteralKind {
    Integer(i32),
}

pub struct Literal {
    pub kind: LiteralKind,
    pub range: Range,
}

impl Literal {
    pub fn new(kind: LiteralKind, range: Range) -> Self {
        Self { kind, range }
    }
}

pub enum BinaryOperator {
    Add,
    Subtract,
    Multiply,
    Divide,
}

pub struct BinaryExpr {
    pub operator: BinaryOperator,
    pub lhs: Box<Expr>,
    pub rhs: Box<Expr>,
    pub range: Range,
}

impl BinaryExpr {
    pub fn new(operator: BinaryOperator, lhs: Box<Expr>, rhs: Box<Expr>, range: Range) -> Self {
        Self {
            operator,
            lhs,
            rhs,
            range,
        }
    }
}

pub enum UnaryOperator {
    Negate,
}

pub struct UnaryExpr {
    pub operator: UnaryOperator,
    pub expr: Box<Expr>,
    pub range: Range,
}

impl UnaryExpr {
    pub fn new(operator: UnaryOperator, expr: Box<Expr>, range: Range) -> Self {
        Self {
            operator,
            expr,
            range,
        }
    }
}

pub struct RetExpr {
    pub expr: Box<Expr>,
    pub range: Range,
}
impl RetExpr {
    pub fn new(expr: Box<Expr>, range: Range) -> Self {
        Self { expr, range }
    }
}
