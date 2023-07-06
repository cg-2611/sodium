use std::ffi::{c_char, c_ulonglong, CString};

use llvm_sys::analysis::{LLVMVerifierFailureAction, LLVMVerifyFunction};
use llvm_sys::core::{
    LLVMAddFunction, LLVMAppendBasicBlockInContext, LLVMBuildRet, LLVMConstInt, LLVMContextCreate,
    LLVMContextDispose, LLVMCreateBuilderInContext, LLVMDisposeBuilder, LLVMDisposeModule,
    LLVMFunctionType, LLVMInt32TypeInContext, LLVMModuleCreateWithNameInContext,
    LLVMPositionBuilderAtEnd, LLVMPrintModuleToFile, LLVMPrintModuleToString,
};
use llvm_sys::prelude::{
    LLVMBasicBlockRef, LLVMBool, LLVMBuilderRef, LLVMContextRef, LLVMModuleRef, LLVMTypeRef,
    LLVMValueRef,
};

const LLVM_TRUE: LLVMBool = 1;
const LLVM_FALSE: LLVMBool = 0;

pub struct Context(LLVMContextRef);

impl Default for Context {
    fn default() -> Self {
        let context = unsafe { LLVMContextCreate() };
        Self(context)
    }
}

#[derive(Clone)]
pub struct Module(LLVMModuleRef);

impl Module {
    pub fn new(module_id: &str, context: &Context) -> Self {
        let module = unsafe {
            let id = c_string_from(module_id);
            LLVMModuleCreateWithNameInContext(id, context.0)
        };

        Self(module)
    }

    pub fn print(&self) {
        let string = unsafe { LLVMPrintModuleToString(self.0) };
        let string = unsafe { CString::from_raw(string) };

        println!("{:?}", string);

        let string = match string.to_str() {
            Ok(string) => string,
            Err(e) => {
                println!("{:?}", e);
                ""
            }
        };

        println!("{:?}", string);
    }

    pub fn write_to_file(&self, file_name: &str) {
        let file_name = c_string_from(file_name);
        unsafe {
            LLVMPrintModuleToFile(self.0, file_name, std::ptr::null_mut());
        }
    }
}

pub struct Builder(LLVMBuilderRef);

impl Builder {
    pub fn new(context: &Context) -> Self {
        let builder = unsafe { LLVMCreateBuilderInContext(context.0) };
        Self(builder)
    }
}

pub struct Value(LLVMValueRef);
pub struct Type(LLVMTypeRef);
pub struct BasicBlock(LLVMBasicBlockRef);

pub fn c_string_from(string: &str) -> *const c_char {
    let c_string = CString::new(string).unwrap();
    c_string.into_raw()
}

pub fn llvm_int32_type(context: &Context) -> Type {
    unsafe { Type(LLVMInt32TypeInContext(context.0)) }
}

pub fn llvm_function_type(ret_type: Type) -> Type {
    unsafe {
        Type(LLVMFunctionType(
            ret_type.0,
            std::ptr::null_mut(),
            0,
            LLVM_FALSE,
        ))
    }
}

pub fn llvm_add_function(module: &Module, name: &str, fn_type: Type) -> Value {
    let function_name = c_string_from(name);
    unsafe { Value(LLVMAddFunction(module.0, function_name, fn_type.0)) }
}

pub fn llvm_add_block(context: &Context, fn_value: &Value, name: &str) -> BasicBlock {
    let name = c_string_from(name);
    unsafe { BasicBlock(LLVMAppendBasicBlockInContext(context.0, fn_value.0, name)) }
}

pub fn llvm_const_int(int_type: Type, x: i32, signed: bool) -> Value {
    let x = x as c_ulonglong;
    let signed = signed.into();
    unsafe { Value(LLVMConstInt(int_type.0, x, signed)) }
}

pub fn llvm_build_ret(builder: &Builder, value: &Value) -> Value {
    unsafe { Value(LLVMBuildRet(builder.0, value.0)) }
}

pub fn llvm_verify_function(fn_value: &Value) -> bool {
    let verified = unsafe {
        LLVMVerifyFunction(
            fn_value.0,
            LLVMVerifierFailureAction::LLVMPrintMessageAction,
        )
    };

    verified == LLVM_TRUE
}

pub fn llvm_position_builder_at_end(builder: &Builder, block: &BasicBlock) {
    unsafe { LLVMPositionBuilderAtEnd(builder.0, block.0) }
}

pub fn llvm_dispose_context(context: &Context) {
    unsafe { LLVMContextDispose(context.0) }
}

pub fn llvm_dispose_module(module: &Module) {
    unsafe { LLVMDisposeModule(module.0) }
}

pub fn llvm_dispose_builder(builder: &Builder) {
    unsafe { LLVMDisposeBuilder(builder.0) }
}
