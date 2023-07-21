use crate::llvm::LLVMString;

pub type LLVMResult<T> = Result<T, LLVMString>;
