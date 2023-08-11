use range::Range;
use ty::Type;

use crate::Expr;

pub enum StmtKind {
    ExprStmt(Box<Expr>),
}

pub struct Stmt {
    pub kind: StmtKind,
    pub ty: Type,
    pub range: Range,
}

impl Stmt {
    pub fn new(kind: StmtKind, ty: Type, range: Range) -> Self {
        Self { kind, ty, range }
    }
}
