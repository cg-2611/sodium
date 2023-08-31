use crate::ir::{Decl, SourceFile};

pub mod decl;
pub mod expr;
pub mod stmt;

#[derive(Default)]
pub struct ASTLower;

impl<'cx, 'ast> ASTLower {
    pub fn new() -> Self {
        Self
    }

    pub fn lower_source_file(
        &self,
        source_file: &'ast ast::SourceFile,
        decls: Vec<Box<Decl<'cx>>>,
    ) -> SourceFile<'cx> {
        SourceFile::new(decls, source_file.range)
    }
}
