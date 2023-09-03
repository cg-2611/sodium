use crate::{BasicBlock, GetRef, LLVMContext, LLVMString, Value};
use llvm_sys::core::{
    LLVMBuildAdd, LLVMBuildMul, LLVMBuildNeg, LLVMBuildRet, LLVMBuildRetVoid, LLVMBuildSub,
    LLVMDisposeBuilder, LLVMPositionBuilderAtEnd,
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

    pub fn build_int_add(&self, lhs: Value, rhs: Value) -> Value {
        let name = LLVMString::from("int_add");
        unsafe {
            Value::new(LLVMBuildAdd(
                self.builder,
                lhs.get_ref(),
                rhs.get_ref(),
                name.ptr,
            ))
        }
    }

    pub fn build_int_subtract(&self, lhs: Value, rhs: Value) -> Value {
        let name = LLVMString::from("int_subtract");
        unsafe {
            Value::new(LLVMBuildSub(
                self.builder,
                lhs.get_ref(),
                rhs.get_ref(),
                name.ptr,
            ))
        }
    }

    pub fn build_int_multiply(&self, lhs: Value, rhs: Value) -> Value {
        let name = LLVMString::from("int_multiply");
        unsafe {
            Value::new(LLVMBuildMul(
                self.builder,
                lhs.get_ref(),
                rhs.get_ref(),
                name.ptr,
            ))
        }
    }

    pub fn build_int_negation(&self, value: Value) -> Value {
        let name = LLVMString::from("int_negation");
        unsafe { Value::new(LLVMBuildNeg(self.builder, value.get_ref(), name.ptr)) }
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
