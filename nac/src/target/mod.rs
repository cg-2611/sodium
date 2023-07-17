use llvm_sys::target_machine::LLVMCodeGenOptLevel;
use llvm_sys::target_machine::LLVMCodeModel;
use llvm_sys::target_machine::LLVMRelocMode;

use crate::errors::Result;
use crate::llvm::{Module, Target, TargetMachine, TargetTriple};
use crate::session::Session;
use crate::target::diagnostics::{target_machine_error, target_triple_error};

pub mod diagnostics;
pub mod link;

pub struct TargetGen<'s, 'ctx> {
    session: &'s Session,
    module: &'ctx Module<'ctx>,
    target_machine: TargetMachine,
}

impl<'s, 'ctx> TargetGen<'s, 'ctx> {
    pub fn new(session: &'s Session, module: &'ctx Module) -> Result<Self> {
        let target_machine = Self::create_target_machine()?;

        Ok(Self {
            session,
            module,
            target_machine,
        })
    }

    pub fn compile_module(session: &'s Session, module: &'ctx Module) -> Result<()> {
        let target_gen = match Self::new(session, module) {
            Ok(target_gen) => target_gen,
            Err(diagnostic) => return Err(diagnostic),
        };

        let result = target_gen.link();
        if let Some(diagnostic) = result.err() {
            target_gen.session.report_diagnostic(diagnostic)
        }

        Ok(())
    }

    pub fn create_target_machine() -> Result<TargetMachine> {
        Target::initialise_all();
        let target_triple = TargetTriple::get_default_triple();
        let target = Target::from_triple(&target_triple).map_err(target_triple_error)?;

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
            .ok_or(target_machine_error())?;

        Ok(target_machine)
    }
}
