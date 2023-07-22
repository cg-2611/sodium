use std::marker::PhantomData;

use llvm_sys::core::{
    LLVMBuildRet, LLVMBuildRetVoid, LLVMDisposeBuilder, LLVMPositionBuilderAtEnd,
};
use llvm_sys::prelude::LLVMBuilderRef;

use crate::llvm::{BasicBlock, Context, GetRef, Value};

pub struct Builder<'ctx> {
    builder: LLVMBuilderRef,
    _marker: PhantomData<&'ctx Context>,
}

impl<'ctx> Builder<'ctx> {
    pub fn new(builder: LLVMBuilderRef) -> Self {
        Self {
            builder,
            _marker: PhantomData,
        }
    }

    pub fn build_ret(&self, value: Option<Value>) -> Value {
        unsafe {
            match value {
                Some(value) => Value::new(LLVMBuildRet(self.get_ref(), value.get_ref())),
                None => Value::new(LLVMBuildRetVoid(self.get_ref())),
            }
        }
    }

    pub fn position_at_end(&self, block: &BasicBlock) {
        unsafe { LLVMPositionBuilderAtEnd(self.get_ref(), block.get_ref()) }
    }
}

impl<'ctx> GetRef<LLVMBuilderRef> for Builder<'ctx> {
    fn get_ref(&self) -> LLVMBuilderRef {
        self.builder
    }
}

impl<'ctx> Drop for Builder<'ctx> {
    fn drop(&mut self) {
        unsafe { LLVMDisposeBuilder(self.get_ref()) }
    }
}
