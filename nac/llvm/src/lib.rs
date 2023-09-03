use std::ffi::{c_char, CStr};
use std::fmt;
use std::fmt::{Display, Formatter};

use llvm_sys::core::{LLVMCreateMessage, LLVMDisposeMessage};

pub use self::basic_block::BasicBlock;
pub use self::builder::LLVMBuilder;
pub use self::context::LLVMContext;
pub use self::diagnostics::LLVMResult;
pub use self::module::LLVMModule;
pub use self::target::{Target, TargetMachine, TargetTriple};
pub use self::types::Type;
pub use self::values::Value;

pub mod basic_block;
pub mod builder;
pub mod context;
pub mod diagnostics;
pub mod module;
pub mod target;
pub mod types;
pub mod values;

pub trait GetRef<T> {
    fn get_ref(&self) -> T;
}

pub struct LLVMString {
    pub ptr: *const c_char,
}

impl LLVMString {
    pub fn new(ptr: *const c_char) -> Self {
        Self { ptr }
    }

    pub fn from(string: &str) -> Self {
        unsafe { LLVMString::new(LLVMCreateMessage(string.as_ptr() as *const c_char)) }
    }

    pub fn as_string(&self) -> String {
        let c_str = unsafe { CStr::from_ptr(self.ptr) };
        c_str.to_string_lossy().into_owned()
    }

    pub fn get_ptr(&self) -> *const c_char {
        self.ptr
    }
}
impl Display for LLVMString {
    fn fmt(&self, f: &mut Formatter) -> Result<(), fmt::Error> {
        write!(f, "{:?}", self.as_string())
    }
}

impl Drop for LLVMString {
    fn drop(&mut self) {
        unsafe {
            LLVMDisposeMessage(self.ptr as *mut _);
        }
    }
}
