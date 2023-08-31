use crate::ir::{Block, Decl, DeclKind, FnDecl};
use crate::lower::ASTLower;
use crate::ty::Type;

impl<'cx, 'ast> ASTLower {
    pub fn lower_decl(
        &self,
        decl: &'ast ast::Decl,
        kind: DeclKind<'cx>,
        ty: Type<'cx>,
    ) -> Decl<'cx> {
        Decl::new(kind, ty, decl.range)
    }

    pub fn lower_fn_decl(
        &self,
        fn_decl: &'ast ast::FnDecl,
        body: Box<Block<'cx>>,
        ty: Type<'cx>,
    ) -> FnDecl<'cx> {
        FnDecl::new(fn_decl.ident, body, ty, fn_decl.range)
    }
}
