use range::Range;

use crate::stmt::Stmt;

pub enum ExprKind {
    Block(Box<Block>),
    Ret(Box<RetExpr>),
    Literal(Literal),
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

pub struct RetExpr {
    pub expr: Box<Expr>,
    pub range: Range,
}
impl RetExpr {
    pub fn new(expr: Box<Expr>, range: Range) -> Self {
        Self { expr, range }
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
