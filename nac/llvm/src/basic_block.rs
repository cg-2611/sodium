use llvm_sys::prelude::LLVMBasicBlockRef;

use crate::GetRef;

pub struct BasicBlock(LLVMBasicBlockRef);

impl BasicBlock {
    pub fn new(basic_block: LLVMBasicBlockRef) -> Self {
        Self(basic_block)
    }
}

impl GetRef<LLVMBasicBlockRef> for BasicBlock {
    fn get_ref(&self) -> LLVMBasicBlockRef {
        self.0
    }
}
