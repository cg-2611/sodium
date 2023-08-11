use std::fmt::{Display, Formatter};

pub use self::context::TypeContext;

pub mod context;

#[derive(Clone, Debug, PartialEq)]
pub enum TypeKind {
    I32,
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
}

impl Display for Type {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "type: {:?}", self.kind)
    }
}
