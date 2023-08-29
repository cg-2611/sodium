use crate::{BasicBlock, GetRef, LLVMContext, Value};
use llvm_sys::core::{
    LLVMBuildRet, LLVMBuildRetVoid, LLVMDisposeBuilder, LLVMPositionBuilderAtEnd,
};
use llvm_sys::prelude::LLVMBuilderRef;
use std::marker::PhantomData;

pub struct LLVMBuilder<'cx> {
    builder: LLVMBuilderRef,
    _marker: PhantomData<&'cx LLVMContext>,
}

impl<'cx> LLVMBuilder<'cx> {
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

impl<'cx> GetRef<LLVMBuilderRef> for LLVMBuilder<'cx> {
    fn get_ref(&self) -> LLVMBuilderRef {
        self.builder
    }
}

impl<'cx> Drop for LLVMBuilder<'cx> {
    fn drop(&mut self) {
        unsafe { LLVMDisposeBuilder(self.get_ref()) }
    }
}
