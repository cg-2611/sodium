use llvm::target::{LLVMCodeGenOptLevel, LLVMCodeModel, LLVMRelocMode};
use llvm::{LLVMModule, Target, TargetMachine, TargetTriple};
use session::Session;

pub use self::diagnostics::{TargetGenError, TargetGenResult};

pub mod diagnostics;
pub mod link;

pub struct TargetGen<'cx> {
    sess: &'cx Session,
    module: &'cx LLVMModule<'cx>,
    target_machine: TargetMachine,
}

impl<'cx> TargetGen<'cx> {
    pub fn new(sess: &'cx Session, module: &'cx LLVMModule) -> TargetGenResult<'cx, Self> {
        let target_machine = Self::create_target_machine(sess)?;

        Ok(Self {
            sess,
            module,
            target_machine,
        })
    }

    pub fn compile_module(sess: &'cx Session, module: &'cx LLVMModule) -> TargetGenResult<'cx, ()> {
        let target_gen = Self::new(sess, module)?;
        target_gen.link()
    }

    pub fn create_target_machine(sess: &'cx Session) -> TargetGenResult<'cx, TargetMachine> {
        Target::initialise_all();
        let target_triple = TargetTriple::get_default_triple();
        let target = Target::from_triple(&target_triple).map_err(|message| {
            let message = format!(
                "failed to get target triple for target generation: {}",
                message
            );

            sess.create_error(message)
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
                sess.create_error("error creating target machine for target generation".to_string())
            })?;

        Ok(target_machine)
    }
}
