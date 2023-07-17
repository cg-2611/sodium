use std::ffi::{c_char, CStr};
use std::fmt;
use std::fmt::{Debug, Display, Formatter};

use llvm_sys::core::{LLVMCreateMessage, LLVMDisposeMessage};

pub use self::basic_block::*;
pub use self::builder::*;
pub use self::context::*;
pub use self::module::*;
pub use self::target::*;
pub use self::types::*;
pub use self::values::*;

pub mod basic_block;
pub mod builder;
pub mod context;
pub mod diagnostics;
pub mod module;
pub mod target;
pub mod types;
pub mod values;

pub type LLVMResult<T> = Result<T, LLVMString>;

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

impl Debug for LLVMString {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
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
