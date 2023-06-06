use crate::ast::expr::RetExpr;
use crate::source::Range;

pub enum StmtKind {
    RetExpr(RetExpr),
}

pub struct Stmt {
    kind: StmtKind,
    range: Range,
}

impl Stmt {
    pub fn new(kind: StmtKind, range: Range) -> Self {
        Self { kind, range }
    }

    pub fn kind(&self) -> &StmtKind {
        &self.kind
    }

    pub fn range(&self) -> Range {
        self.range
    }
}
