use range::Range;

use crate::Expr;

pub enum StmtKind {
    ExprStmt(Expr),
}

pub struct Stmt {
    pub kind: StmtKind,
    pub range: Range,
}

impl Stmt {
    pub fn new(kind: StmtKind, range: Range) -> Self {
        Self { kind, range }
    }
}
