use llvm_sys::analysis::{LLVMVerifierFailureAction, LLVMVerifyFunction};
use llvm_sys::prelude::LLVMValueRef;

use crate::llvm::{GetRef, LLVMResult, LLVMString};

pub struct Value(LLVMValueRef);

impl Value {
    pub fn new(value: LLVMValueRef) -> Self {
        Self(value)
    }

    pub fn verify_fn(&self) -> LLVMResult<()> {
        let error = unsafe {
            LLVMVerifyFunction(
                self.get_ref(),
                LLVMVerifierFailureAction::LLVMReturnStatusAction,
            )
        };

        if error == true.into() {
            return Err(LLVMString::from("failed to verify function"));
        }

        Ok(())
    }
}

impl GetRef<LLVMValueRef> for Value {
    fn get_ref(&self) -> LLVMValueRef {
        self.0
    }
}
