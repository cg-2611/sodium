use std::marker::PhantomData;
use std::mem::MaybeUninit;

use llvm_sys::analysis::{LLVMVerifierFailureAction, LLVMVerifyModule};
use llvm_sys::core::{LLVMAddFunction, LLVMDisposeModule, LLVMDumpModule, LLVMPrintModuleToFile};
use llvm_sys::prelude::LLVMModuleRef;

use crate::llvm::{Context, GetRef, LLVMResult, LLVMString, Type, Value};

pub struct Module<'ctx> {
    module: LLVMModuleRef,
    _marker: PhantomData<&'ctx Context>,
}

impl<'ctx> Module<'ctx> {
    pub fn new(module: LLVMModuleRef) -> Self {
        Self {
            module,
            _marker: PhantomData,
        }
    }

    pub fn dispose(&self) {
        unsafe { LLVMDisposeModule(self.get_ref()) }
    }

    pub fn print(&self) {
        unsafe { LLVMDumpModule(self.get_ref()) }
    }

    pub fn write_to_file(&self, file_name: &str) -> LLVMResult<()> {
        let file_name = LLVMString::from(file_name);
        let mut error_string = MaybeUninit::uninit();
        let error = unsafe {
            LLVMPrintModuleToFile(self.get_ref(), file_name.ptr, error_string.as_mut_ptr())
        };

        let error_string = unsafe { error_string.assume_init() };
        if error == true.into() && !error_string.is_null() {
            return Err(LLVMString::new(error_string));
        }

        Ok(())
    }

    pub fn add_function(&self, name: &str, fn_type: &Type) -> Value {
        let name = LLVMString::from(name);
        unsafe { Value::new(LLVMAddFunction(self.get_ref(), name.ptr, fn_type.get_ref())) }
    }

    pub fn verify(&self) -> LLVMResult<()> {
        let mut error_string = MaybeUninit::uninit();
        let error = unsafe {
            LLVMVerifyModule(
                self.get_ref(),
                LLVMVerifierFailureAction::LLVMReturnStatusAction,
                error_string.as_mut_ptr(),
            )
        };

        let error_string = unsafe { error_string.assume_init() };
        if error == true.into() && !error_string.is_null() {
            return Err(LLVMString::new(error_string));
        }

        Ok(())
    }
}

impl<'ctx> GetRef<LLVMModuleRef> for Module<'ctx> {
    fn get_ref(&self) -> LLVMModuleRef {
        self.module
    }
}

impl<'ctx> Drop for Module<'ctx> {
    fn drop(&mut self) {
        unsafe { LLVMDisposeModule(self.get_ref()) }
    }
}
