use std::ffi::CString;

use llvm_sys::target::{
    LLVM_InitializeAllAsmParsers, LLVM_InitializeAllAsmPrinters, LLVM_InitializeAllDisassemblers,
    LLVM_InitializeAllTargetInfos, LLVM_InitializeAllTargetMCs, LLVM_InitializeAllTargets,
};
use llvm_sys::target_machine::{
    LLVMCodeGenFileType, LLVMCodeGenOptLevel, LLVMCodeModel, LLVMCreateTargetMachine,
    LLVMDisposeTargetMachine, LLVMGetDefaultTargetTriple, LLVMGetHostCPUFeatures,
    LLVMGetHostCPUName, LLVMGetTargetFromTriple, LLVMRelocMode, LLVMTargetMachineEmitToFile,
    LLVMTargetMachineRef, LLVMTargetRef,
};

use crate::llvm::{c_string_from, mut_c_string_from, Module};

pub struct Target(LLVMTargetRef);
pub struct TargetMachine(LLVMTargetMachineRef);

pub fn llvm_initialise_all_targets() {
    unsafe {
        LLVM_InitializeAllTargets();
        LLVM_InitializeAllTargetInfos();
        LLVM_InitializeAllAsmParsers();
        LLVM_InitializeAllAsmPrinters();
        LLVM_InitializeAllDisassemblers();
        LLVM_InitializeAllTargetMCs();
    }
}

pub fn llvm_get_target_triple() -> String {
    let triple = unsafe { LLVMGetDefaultTargetTriple() };
    let c_string = unsafe { CString::from_raw(triple) };
    c_string.into_string().unwrap()
}

pub fn llvm_get_target_from_triple(triple: &str) -> Target {
    let triple = c_string_from(triple);
    let mut target: LLVMTargetRef = std::ptr::null_mut();

    unsafe {
        LLVMGetTargetFromTriple(triple, &mut target, std::ptr::null_mut());
    }

    Target(target)
}

pub fn llvm_get_host_cpu() -> String {
    let cpu = unsafe { LLVMGetHostCPUName() };
    let c_string = unsafe { CString::from_raw(cpu) };
    c_string.into_string().unwrap()
}

pub fn llvm_get_host_cpu_features() -> String {
    let features = unsafe { LLVMGetHostCPUFeatures() };
    let c_string = unsafe { CString::from_raw(features) };
    c_string.into_string().unwrap()
}

pub fn llvm_create_target_machine(
    target: &Target,
    triple: &str,
    cpu: &str,
    features: &str,
    level: LLVMCodeGenOptLevel,
    reloc: LLVMRelocMode,
    code_model: LLVMCodeModel,
) -> TargetMachine {
    let triple = c_string_from(triple);
    let cpu = c_string_from(cpu);
    let features = c_string_from(features);

    let target_machine = unsafe {
        LLVMCreateTargetMachine(target.0, triple, cpu, features, level, reloc, code_model)
    };
    TargetMachine(target_machine)
}

pub fn llvm_dispose_target_machine(machine: &TargetMachine) {
    unsafe { LLVMDisposeTargetMachine(machine.0) }
}

pub fn llvm_emit_to_object_file(machine: &TargetMachine, module: &Module, path: &str) {
    let path = mut_c_string_from(path);

    unsafe {
        LLVMTargetMachineEmitToFile(
            machine.0,
            module.get_ref(),
            path,
            LLVMCodeGenFileType::LLVMObjectFile,
            std::ptr::null_mut(),
        )
    };
}
