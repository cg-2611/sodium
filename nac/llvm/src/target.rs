use std::ffi::c_char;
use std::mem::MaybeUninit;

use llvm_sys::target::{
    LLVM_InitializeAllAsmParsers, LLVM_InitializeAllAsmPrinters, LLVM_InitializeAllDisassemblers,
    LLVM_InitializeAllTargetInfos, LLVM_InitializeAllTargetMCs, LLVM_InitializeAllTargets,
};
use llvm_sys::target_machine::{
    LLVMCodeGenFileType, LLVMCreateTargetMachine, LLVMDisposeTargetMachine,
    LLVMGetDefaultTargetTriple, LLVMGetHostCPUFeatures, LLVMGetHostCPUName,
    LLVMGetTargetFromTriple, LLVMTargetMachineEmitToFile, LLVMTargetMachineRef, LLVMTargetRef,
};

use crate::{GetRef, LLVMModule, LLVMResult, LLVMString};

pub use llvm_sys::target_machine::LLVMCodeGenOptLevel;
pub use llvm_sys::target_machine::LLVMCodeModel;
pub use llvm_sys::target_machine::LLVMRelocMode;

pub struct Target(LLVMTargetRef);

impl Target {
    pub fn new(target: LLVMTargetRef) -> Self {
        Self(target)
    }

    pub fn initialise_all() {
        unsafe {
            LLVM_InitializeAllTargets();
            LLVM_InitializeAllTargetInfos();
            LLVM_InitializeAllAsmParsers();
            LLVM_InitializeAllAsmPrinters();
            LLVM_InitializeAllDisassemblers();
            LLVM_InitializeAllTargetMCs();
        }
    }

    pub fn from_triple(triple: &TargetTriple) -> LLVMResult<Self> {
        let mut target = std::ptr::null_mut();
        let mut error_string = MaybeUninit::uninit();

        let error = unsafe {
            LLVMGetTargetFromTriple(triple.get_ptr(), &mut target, error_string.as_mut_ptr())
        };

        let error_string = unsafe { error_string.assume_init() };
        if error == true.into() && !error_string.is_null() {
            return Err(LLVMString::new(error_string));
        }

        Ok(Target::new(target))
    }

    pub fn create_target_machine(
        &self,
        triple: &TargetTriple,
        cpu: &LLVMString,
        features: &LLVMString,
        level: LLVMCodeGenOptLevel,
        reloc: LLVMRelocMode,
        code_model: LLVMCodeModel,
    ) -> Option<TargetMachine> {
        let target_machine = unsafe {
            LLVMCreateTargetMachine(
                self.get_ref(),
                triple.get_ptr(),
                cpu.get_ptr(),
                features.get_ptr(),
                level,
                reloc,
                code_model,
            )
        };

        if target_machine.is_null() {
            return None;
        }

        Some(TargetMachine::new(target_machine))
    }
}

impl GetRef<LLVMTargetRef> for Target {
    fn get_ref(&self) -> LLVMTargetRef {
        self.0
    }
}

pub struct TargetMachine(LLVMTargetMachineRef);

impl TargetMachine {
    pub fn new(target: LLVMTargetMachineRef) -> Self {
        Self(target)
    }

    pub fn get_host_cpu_name() -> LLVMString {
        unsafe { LLVMString::new(LLVMGetHostCPUName()) }
    }

    pub fn get_host_cpu_features() -> LLVMString {
        let features = unsafe { LLVMGetHostCPUFeatures() };
        LLVMString::new(features)
    }

    pub fn write_to_file(&self, module: &LLVMModule, path: &str) -> LLVMResult<()> {
        let path = LLVMString::from(path);
        let mut error_string = MaybeUninit::uninit();

        let error = unsafe {
            let path = path.get_ptr() as *mut _;

            LLVMTargetMachineEmitToFile(
                self.get_ref(),
                module.get_ref(),
                path,
                LLVMCodeGenFileType::LLVMObjectFile,
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

impl GetRef<LLVMTargetMachineRef> for TargetMachine {
    fn get_ref(&self) -> LLVMTargetMachineRef {
        self.0
    }
}

impl Drop for TargetMachine {
    fn drop(&mut self) {
        unsafe { LLVMDisposeTargetMachine(self.get_ref()) }
    }
}

pub struct TargetTriple(LLVMString);

impl TargetTriple {
    pub fn new(triple: LLVMString) -> Self {
        Self(triple)
    }

    pub fn create(triple: &str) -> Self {
        let triple = LLVMString::from(triple);
        TargetTriple::new(triple)
    }

    pub fn get_default_triple() -> Self {
        let triple = unsafe { LLVMString::new(LLVMGetDefaultTargetTriple()) };
        TargetTriple::new(triple)
    }

    pub fn get_ptr(&self) -> *const c_char {
        self.0.get_ptr()
    }
}
