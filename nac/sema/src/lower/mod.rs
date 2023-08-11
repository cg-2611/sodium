use ast::AST;
use context::CompilerContext;
use ir::{Identifier, SourceFile, IR};
use ty::Type;

use crate::inference::TypeInference;
use crate::SemaResult;

pub mod decl;
pub mod expr;
pub mod stmt;

pub struct ASTLower<'ctx> {
    context: &'ctx CompilerContext<'ctx>,
    type_inference: TypeInference<'ctx>,
}

impl<'ctx, 'ast> ASTLower<'ctx> {
    pub fn new(context: &'ctx CompilerContext) -> Self {
        let type_inference = TypeInference::new(context);
        Self {
            context,
            type_inference,
        }
    }

    pub fn lower(context: &'ctx CompilerContext, ast: &'ast AST) -> SemaResult<'ctx, IR> {
        let ast_lower = ASTLower::new(context);
        let source_file = ast_lower.lower_source_file(ast.root())?;
        Ok(IR::new(source_file))
    }

    pub fn lower_source_file(
        &self,
        source_file: &'ast ast::SourceFile,
    ) -> SemaResult<'ctx, SourceFile> {
        let mut decls = Vec::new();

        for ast_decl in &source_file.decls {
            let decl = self.lower_decl(ast_decl)?;
            decls.push(Box::new(decl));
        }

        Ok(SourceFile::new(decls, source_file.range))
    }

    pub fn lower_ident(&self, ident: &'ast ast::Identifier) -> SemaResult<'ctx, Identifier> {
        Ok(Identifier::new(ident.value.clone(), ident.range))
    }

    pub fn lower_type(&self, ty: &'ast ast::Type) -> SemaResult<'ctx, Type> {
        self.type_inference.ast_type_to_type(ty)
    }
}
