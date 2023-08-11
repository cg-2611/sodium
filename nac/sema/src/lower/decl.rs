use ir::{Decl, DeclKind, FnDecl};

use crate::lower::ASTLower;
use crate::SemaResult;

impl<'ctx, 'ast> ASTLower<'ctx> {
    pub fn lower_decl(&self, decl: &'ast ast::Decl) -> SemaResult<'ctx, Decl> {
        match &decl.kind {
            ast::DeclKind::Fn(ast_fn_decl) => {
                let fn_decl = self.lower_fn_decl(ast_fn_decl)?;
                let fn_decl_ty = fn_decl.ty.clone();
                Ok(Decl::new(
                    DeclKind::Fn(Box::new(fn_decl)),
                    fn_decl_ty,
                    ast_fn_decl.range,
                ))
            }
        }
    }

    pub fn lower_fn_decl(&self, fn_decl: &'ast ast::FnDecl) -> SemaResult<'ctx, FnDecl> {
        let ident = self.lower_ident(&fn_decl.ident)?;
        let ret_type = self.lower_type(&fn_decl.ret_type)?;
        let body = self.lower_block(&fn_decl.body)?;

        let fn_decl_ty = self.context.type_context().fn_ty(ret_type);

        Ok(FnDecl::new(ident, body, fn_decl_ty, fn_decl.range))
    }
}
