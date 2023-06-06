pub mod decl;
pub mod expr;
pub mod stmt;
pub mod ty;

use crate::ast::decl::Decl;
use crate::source::Range;

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
    decls: Vec<Decl>,
    range: Range,
}

impl SourceFile {
    pub fn new(decls: Vec<Decl>, range: Range) -> Self {
        Self { decls, range }
    }

    pub fn decls(&self) -> &Vec<Decl> {
        &self.decls
    }

    pub fn range(&self) -> Range {
        self.range
    }
}

pub struct Identifier {
    value: String,
    range: Range,
}

impl Identifier {
    pub fn new(value: String, range: Range) -> Self {
        Self { value, range }
    }

    pub fn value(&self) -> &String {
        &self.value
    }

    pub fn range(&self) -> Range {
        self.range
    }
}
