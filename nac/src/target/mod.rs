use llvm_sys::target_machine::LLVMCodeGenOptLevel;
use llvm_sys::target_machine::LLVMCodeModel;
use llvm_sys::target_machine::LLVMRelocMode;

use crate::llvm::{Module, Target, TargetMachine, TargetTriple};
use crate::session::Session;

pub use self::diagnostics::{TargetGenError, TargetGenResult};

pub mod diagnostics;
pub mod link;

pub struct TargetGen<'a, 'ctx> {
    session: &'a Session,
    module: &'ctx Module<'ctx>,
    target_machine: TargetMachine,
}

impl<'a, 'ctx> TargetGen<'a, 'ctx> {
    pub fn new(session: &'a Session, module: &'ctx Module) -> TargetGenResult<'a, Self> {
        let target_machine = Self::create_target_machine(session)?;

        Ok(Self {
            session,
            module,
            target_machine,
        })
    }

    pub fn compile_module(session: &'a Session, module: &'ctx Module) -> TargetGenResult<'a, ()> {
        let target_gen = Self::new(session, module)?;
        target_gen.link()
    }

    pub fn create_target_machine(session: &'a Session) -> TargetGenResult<'a, TargetMachine> {
        Target::initialise_all();
        let target_triple = TargetTriple::get_default_triple();
        let target = Target::from_triple(&target_triple).map_err(|message| {
            let message = format!(
                "failed to get target triple for target generation: {}",
                message
            );

            session.create_error(message)
        })?;

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
            .ok_or_else(|| {
                session
                    .create_error("error creating target machine for target generation".to_string())
            })?;

        Ok(target_machine)
    }
}
