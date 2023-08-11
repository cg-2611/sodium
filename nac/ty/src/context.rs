use crate::Type;

pub struct TypeContext {
    i32: Type,
}

impl TypeContext {
    pub fn new() -> Self {
        Self { i32: Type::i32() }
    }

    pub fn i32(&self) -> Type {
        self.i32.clone()
    }
}

impl Default for TypeContext {
    fn default() -> Self {
        Self::new()
    }
}
