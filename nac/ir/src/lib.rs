use range::Range;

pub use self::decl::{Decl, DeclKind, FnDecl};
pub use self::expr::{Block, Expr, ExprKind, Literal, LiteralKind, RetExpr};
pub use self::stmt::{Stmt, StmtKind};

pub mod decl;
pub mod expr;
pub mod print;
pub mod stmt;

pub struct IR {
    source_file: SourceFile,
}

impl IR {
    pub fn new(source_file: SourceFile) -> Self {
        Self { source_file }
    }

    pub fn source_file(&self) -> &SourceFile {
        &self.source_file
    }
}

pub struct SourceFile {
    pub decls: Vec<Box<Decl>>,
    pub range: Range,
}

impl SourceFile {
    pub fn new(decls: Vec<Box<Decl>>, range: Range) -> Self {
        Self { decls, range }
    }
}

pub struct Identifier {
    pub value: String,
    pub range: Range,
}

impl Identifier {
    pub fn new(value: String, range: Range) -> Self {
        Self { value, range }
    }
}
