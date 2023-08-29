use crate::ir::{Stmt, StmtKind};
use crate::lower::ASTLower;
use crate::ty::Type;

impl<'cx, 'ast> ASTLower {
    pub fn lower_stmt(
        &self,
        stmt: &'ast ast::Stmt,
        kind: StmtKind<'cx>,
        ty: Type<'cx>,
    ) -> Stmt<'cx> {
        Stmt::new(kind, ty, stmt.range)
    }
}
