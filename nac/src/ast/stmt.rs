use crate::ast::expr::Expr;
use crate::source::Range;

pub enum StmtKind {
    ExprStmt(Box<Expr>),
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
