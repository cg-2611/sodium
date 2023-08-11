use range::Range;
use ty::Type;

use crate::Stmt;

pub enum ExprKind {
    Block(Box<Block>),
    Ret(Box<RetExpr>),
    Literal(Literal),
}

pub struct Expr {
    pub kind: ExprKind,
    pub ty: Type,
    pub range: Range,
}

impl Expr {
    pub fn new(kind: ExprKind, ty: Type, range: Range) -> Self {
        Self { kind, ty, range }
    }
}

pub struct Block {
    pub stmts: Vec<Stmt>,
    pub ty: Type,
    pub range: Range,
}

impl Block {
    pub fn new(stmts: Vec<Stmt>, ty: Type, range: Range) -> Self {
        Self { stmts, ty, range }
    }
}

pub struct RetExpr {
    pub expr: Box<Expr>,
    pub ty: Type,
    pub range: Range,
}

impl RetExpr {
    pub fn new(expr: Box<Expr>, ty: Type, range: Range) -> Self {
        Self { expr, ty, range }
    }
}

#[derive(Debug, PartialEq)]
pub enum LiteralKind {
    Integer(i32),
}

pub struct Literal {
    pub kind: LiteralKind,
    pub ty: Type,
    pub range: Range,
}

impl Literal {
    pub fn new(kind: LiteralKind, ty: Type, range: Range) -> Self {
        Self { kind, ty, range }
    }
}
