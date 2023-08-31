use range::Range;

pub use self::decl::{Decl, DeclKind, FnDecl};
pub use self::expr::{Block, Expr, ExprKind, Literal, LiteralKind, RetExpr};
pub use self::stmt::{Stmt, StmtKind};

pub mod decl;
pub mod expr;
pub mod print;
pub mod stmt;

pub struct IR<'cx> {
    source_file: SourceFile<'cx>,
}

impl<'cx> IR<'cx> {
    pub fn new(source_file: SourceFile<'cx>) -> Self {
        Self { source_file }
    }

    pub fn source_file(&self) -> &SourceFile {
        &self.source_file
    }
}

pub struct SourceFile<'cx> {
    pub decls: Vec<Box<Decl<'cx>>>,
    pub range: Range,
}

impl<'cx> SourceFile<'cx> {
    pub fn new(decls: Vec<Box<Decl<'cx>>>, range: Range) -> Self {
        Self { decls, range }
    }
}
