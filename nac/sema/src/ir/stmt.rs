use range::Range;

use crate::ir::Expr;
use crate::ty::Type;

pub enum StmtKind<'cx> {
    ExprStmt(Box<Expr<'cx>>),
}

pub struct Stmt<'cx> {
    pub kind: StmtKind<'cx>,
    pub ty: Type<'cx>,
    pub range: Range,
}

impl<'cx> Stmt<'cx> {
    pub fn new(kind: StmtKind<'cx>, ty: Type<'cx>, range: Range) -> Self {
        Self { kind, ty, range }
    }
}
