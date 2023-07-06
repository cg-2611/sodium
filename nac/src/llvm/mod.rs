use std::ffi::{c_char, CString};

use llvm_sys::prelude::LLVMBool;

pub use self::core::*;

pub mod core;

const LLVM_TRUE: LLVMBool = 1;
const LLVM_FALSE: LLVMBool = 0;

pub fn c_string_from(string: &str) -> *const c_char {
    let c_string = CString::new(string).unwrap();
    c_string.into_raw()
}
