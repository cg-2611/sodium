use llvm_sys::target_machine::LLVMCodeGenOptLevel;
use llvm_sys::target_machine::LLVMCodeModel;
use llvm_sys::target_machine::LLVMRelocMode;

use crate::llvm::{Module, Target, TargetMachine, TargetTriple};

pub mod link;

pub struct TargetGen<'ctx> {
    module: &'ctx Module<'ctx>,
}

impl<'ctx> TargetGen<'ctx> {
    pub fn new(module: &'ctx Module) -> Self {
        Self { module }
    }

    pub fn compile_module(module: &'ctx Module) {
        let target_gen = TargetGen::new(module);

        Target::initialise_all();
        let target_triple = TargetTriple::get_default_triple();
        let target = Target::from_triple(&target_triple).unwrap();

        let cpu = TargetMachine::get_host_cpu_name();
        let features = TargetMachine::get_host_cpu_features();

        let target_machine = target
            .create_target_machine(
                &target_triple,
                &cpu,
                &features,
                LLVMCodeGenOptLevel::LLVMCodeGenLevelDefault,
                LLVMRelocMode::LLVMRelocDefault,
                LLVMCodeModel::LLVMCodeModelDefault,
            )
            .unwrap();

        target_gen.link_executable(target_gen.module, &target_machine);
    }
}
