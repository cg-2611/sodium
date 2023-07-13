use llvm_sys::core::{
    LLVMAppendBasicBlockInContext, LLVMContextCreate, LLVMContextDispose,
    LLVMCreateBuilderInContext, LLVMInt32TypeInContext, LLVMModuleCreateWithNameInContext,
};
use llvm_sys::prelude::LLVMContextRef;

use crate::llvm::{BasicBlock, Builder, GetRef, LLVMString, Module, Type, Value};

pub struct Context(LLVMContextRef);

impl Context {
    pub fn new(context: LLVMContextRef) -> Self {
        Self(context)
    }

    pub fn create() -> Self {
        unsafe { Context::new(LLVMContextCreate()) }
    }

    pub fn create_module(&self, name: &str) -> Module {
        let name = LLVMString::from(name);
        unsafe { Module::new(LLVMModuleCreateWithNameInContext(name.ptr, self.get_ref())) }
    }

    pub fn create_builder(&self) -> Builder {
        unsafe { Builder::new(LLVMCreateBuilderInContext(self.get_ref())) }
    }

    pub fn dispose(&self) {
        unsafe { LLVMContextDispose(self.get_ref()) }
    }

    pub fn i32_type(&self) -> Type {
        unsafe { Type::new(LLVMInt32TypeInContext(self.get_ref())) }
    }

    pub fn append_basic_block(&self, fn_value: &Value, name: &str) -> BasicBlock {
        let name = LLVMString::from(name);
        unsafe {
            BasicBlock::new(LLVMAppendBasicBlockInContext(
                self.get_ref(),
                fn_value.get_ref(),
                name.ptr,
            ))
        }
    }
}

impl GetRef<LLVMContextRef> for Context {
    fn get_ref(&self) -> LLVMContextRef {
        self.0
    }
}

impl Drop for Context {
    fn drop(&mut self) {
        unsafe { LLVMContextDispose(self.get_ref()) }
    }
}
