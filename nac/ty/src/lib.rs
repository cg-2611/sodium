use std::fmt::{Display, Formatter};

pub use self::context::TypeContext;

pub mod context;

#[derive(Clone, Debug, PartialEq)]
pub enum TypeKind {
    I32,
    Fn(FnType),
}

#[derive(Clone, Debug, PartialEq)]
pub struct Type {
    pub kind: TypeKind,
}

impl Type {
    pub fn i32() -> Self {
        Self {
            kind: TypeKind::I32,
        }
    }

    pub fn fn_ty(ret_type: Type) -> Self {
        Self {
            kind: TypeKind::Fn(FnType::new(ret_type)),
        }
    }

    pub fn matches(&self, other: &Type) -> bool {
        self.kind == other.kind
    }
}

impl Display for Type {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "type: {:?}", self.kind)
    }
}

#[derive(Clone, Debug, PartialEq)]
pub struct FnType {
    ret_type: Box<Type>,
}

impl FnType {
    pub fn new(ret_type: Type) -> Self {
        Self {
            ret_type: Box::new(ret_type),
        }
    }
}

impl Display for FnType {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "Fn() -> {}", self.ret_type)
    }
}
