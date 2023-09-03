use std::fmt::{Debug, Display, Formatter};

use crate::ty::intern::InternedType;

pub use self::context::{Context, PrimitiveTypes, TypeArena, TypeContext};

pub mod context;
pub mod intern;

#[derive(Debug, Hash, Eq, PartialEq, Copy, Clone)]
pub enum TypeKind<'cx> {
    I32,
    Fn(Type<'cx>),
}

#[derive(Hash, Eq, PartialEq, Copy, Clone)]
pub struct Type<'cx>(InternedType<'cx>);

impl<'a, 'cx: 'a> Type<'cx> {
    pub fn new(ty: InternedType<'cx>) -> Self {
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

    fn as_string(&self) -> String {
        match self.kind() {
            TypeKind::I32 => String::from("i32"),
            TypeKind::Fn(ty) => format!("fn -> {}", ty.as_string()),
        }
    }
}

impl<'cx> Debug for Type<'cx> {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.as_string())
    }
}

impl<'cx> Display for Type<'cx> {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        Debug::fmt(self, f)
    }
}
