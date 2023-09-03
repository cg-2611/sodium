use llvm::target::{LLVMCodeGenOptLevel, LLVMCodeModel, LLVMRelocMode};
use llvm::{LLVMModule, Target, TargetMachine, TargetTriple};
use session::Session;

pub use self::diagnostics::{TargetGenError, TargetGenResult};

pub mod diagnostics;
pub mod link;

pub struct TargetGen<'a> {
    sess: &'a Session,
    module: &'a LLVMModule<'a>,
    target_machine: TargetMachine,
}

impl<'a> TargetGen<'a> {
    pub fn new(sess: &'a Session, module: &'a LLVMModule) -> TargetGenResult<'a, Self> {
        let target_machine = Self::create_target_machine(sess)?;

        Ok(Self {
            sess,
            module,
            target_machine,
        })
    }

    pub fn compile_module(sess: &'a Session, module: &'a LLVMModule) -> TargetGenResult<'a, ()> {
        let target_gen = Self::new(sess, module)?;
        target_gen.link()
    }

    pub fn create_target_machine(sess: &'a Session) -> TargetGenResult<'a, TargetMachine> {
        Target::initialise_all();
        let target_triple = TargetTriple::get_default_triple();
        let target = Target::from_triple(&target_triple).map_err(|message| {
            let message = format!("failed to get target triple: {}", message);

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
            .ok_or_else(|| sess.create_error("error creating target machine".to_string()))?;

        Ok(target_machine)
    }
}
