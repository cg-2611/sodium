use range::Range;

pub use self::decl::{Decl, DeclKind, FnDecl};
pub use self::expr::{Block, Expr, ExprKind, Literal, LiteralKind, RetExpr};
pub use self::stmt::{Stmt, StmtKind};
pub use self::ty::Type;

pub mod decl;
pub mod expr;
pub mod print;
pub mod stmt;
pub mod ty;

pub struct AST {
    root: SourceFile,
}

impl AST {
    pub fn new(root: SourceFile) -> Self {
        Self { root }
    }

    pub fn root(&self) -> &SourceFile {
        &self.root
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
