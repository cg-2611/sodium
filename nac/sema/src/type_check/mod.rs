use context::CompilerContext;
use ir::{Decl, DeclKind, FnDecl, SourceFile, IR};

use crate::SemaResult;

pub struct TypeChecker<'ctx> {
    context: &'ctx CompilerContext<'ctx>,
}

impl<'ctx, 'ir> TypeChecker<'ctx> {
    pub fn new(context: &'ctx CompilerContext) -> Self {
        Self { context }
    }

    pub fn type_check(context: &'ctx CompilerContext, ir: &'ir IR) -> SemaResult<'ctx, ()> {
        let type_checker = TypeChecker::new(context);
        type_checker.type_check_source_file(ir.source_file())
    }

    pub fn type_check_source_file(&self, source_file: &'ir SourceFile) -> SemaResult<'ctx, ()> {
        for decl in &source_file.decls {
            self.type_check_decl(decl)?;
        }

        Ok(())
    }

    pub fn type_check_decl(&self, decl: &'ir Decl) -> SemaResult<'ctx, ()> {
        match &decl.kind {
            DeclKind::Fn(fn_decl) => self.type_check_fn_decl(fn_decl),
        }
    }

    pub fn type_check_fn_decl(&self, fn_decl: &'ir FnDecl) -> SemaResult<'ctx, ()> {
        let fn_ret_type = &fn_decl.ty;
        let fn_body_type = &fn_decl.body.ty;

        if !fn_ret_type.matches(fn_body_type) {
            let message = format!(
                "type mismatch: expected type `{}`, found `{}`",
                fn_ret_type, fn_body_type
            );
            return Err(self
                .context
                .session()
                .create_ranged_error(message, fn_decl.range));
        }

        Ok(())
    }
}
