use data_structures::intern::Interned;

use crate::ty::context::TypeContext;

pub mod context;

#[derive(Debug, Hash, Eq, PartialEq, Copy, Clone)]
pub enum TypeKind<'cx> {
    I32,
    Fn(Type<'cx>),
}

#[derive(Debug, Hash, Eq, PartialEq, Copy, Clone)]
pub struct Type<'cx>(Interned<'cx, TypeKind<'cx>>);

impl<'a, 'cx: 'a> Type<'cx> {
    pub fn new(ty: Interned<'cx, TypeKind<'cx>>) -> Self {
        Self(ty)
    }

    pub fn kind(&'cx self) -> &'cx TypeKind<'cx> {
        &self.0
    }

    pub fn i32(tcx: TypeContext<'a, 'cx>) -> Self {
        tcx.i32()
    }

    pub fn fn_type(tcx: TypeContext<'a, 'cx>, ty: Self) -> Self {
        tcx.fn_type(ty)
    }
}
