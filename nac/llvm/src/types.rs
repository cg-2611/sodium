use llvm_sys::core::{LLVMConstInt, LLVMFunctionType};
use llvm_sys::prelude::LLVMTypeRef;

use crate::{GetRef, Value};

pub struct Type(LLVMTypeRef);

impl Type {
    pub fn new(ty: LLVMTypeRef) -> Self {
        Self(ty)
    }

    pub fn fn_type(&self) -> Type {
        unsafe {
            Type::new(LLVMFunctionType(
                self.0,
                std::ptr::null_mut(),
                0,
                false.into(),
            ))
        }
    }

    pub fn const_int(&self, value: u64) -> Value {
        unsafe { Value::new(LLVMConstInt(self.get_ref(), value, false.into())) }
    }
}

impl GetRef<LLVMTypeRef> for Type {
    fn get_ref(&self) -> LLVMTypeRef {
        self.0
    }
}
