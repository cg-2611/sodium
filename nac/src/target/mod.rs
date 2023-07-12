use llvm_sys::target_machine::LLVMCodeGenOptLevel;
use llvm_sys::target_machine::LLVMCodeModel;
use llvm_sys::target_machine::LLVMRelocMode;

use crate::llvm::{
    llvm_create_target_machine, llvm_dispose_target_machine, llvm_get_host_cpu,
    llvm_get_host_cpu_features, llvm_get_target_from_triple, llvm_get_target_triple,
    llvm_initialise_all_targets, Module, TargetMachine,
};

pub mod link;

pub struct TargetGen<'m> {
    module: &'m Module,
}

impl<'m> TargetGen<'m> {
    pub fn new(module: &'m Module) -> Self {
        Self { module }
    }

    pub fn compile_module(module: &'m Module) {
        let target_gen = TargetGen::new(module);

        llvm_initialise_all_targets();

        let target_machine = target_gen.create_target_machine();
        target_gen.link_executable(target_gen.module, &target_machine);

        llvm_dispose_target_machine(&target_machine);
    }

    pub fn create_target_machine(&self) -> TargetMachine {
        let triple = llvm_get_target_triple();
        let target = llvm_get_target_from_triple(triple.as_str());
        let cpu = llvm_get_host_cpu();
        let features = llvm_get_host_cpu_features();
        llvm_create_target_machine(
            &target,
            triple.as_str(),
            cpu.as_str(),
            features.as_str(),
            LLVMCodeGenOptLevel::LLVMCodeGenLevelDefault,
            LLVMRelocMode::LLVMRelocDefault,
            LLVMCodeModel::LLVMCodeModelDefault,
        )
    }
}
