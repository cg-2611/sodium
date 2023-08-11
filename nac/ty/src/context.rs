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

    pub fn fn_ty(&self, ret_type: Type) -> Type {
        Type::fn_ty(ret_type)
    }
}

impl Default for TypeContext {
    fn default() -> Self {
        Self::new()
    }
}
