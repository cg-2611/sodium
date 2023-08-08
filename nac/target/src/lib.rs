use context::CompilerContext;
use llvm::target::{LLVMCodeGenOptLevel, LLVMCodeModel, LLVMRelocMode};
use llvm::{Module, Target, TargetMachine, TargetTriple};
use session::Session;

pub use self::diagnostics::{TargetGenError, TargetGenResult};

pub mod diagnostics;
pub mod link;

pub struct TargetGen<'ctx> {
    context: &'ctx CompilerContext<'ctx>,
    module: &'ctx Module<'ctx>,
    target_machine: TargetMachine,
}

impl<'ctx> TargetGen<'ctx> {
    pub fn new(
        context: &'ctx CompilerContext,
        module: &'ctx Module,
    ) -> TargetGenResult<'ctx, Self> {
        let target_machine = Self::create_target_machine(context.session())?;

        Ok(Self {
            context,
            module,
            target_machine,
        })
    }

    pub fn compile_module(
        context: &'ctx CompilerContext,
        module: &'ctx Module,
    ) -> TargetGenResult<'ctx, ()> {
        let target_gen = Self::new(context, module)?;
        target_gen.link()
    }

    pub fn create_target_machine(session: &'ctx Session) -> TargetGenResult<'ctx, TargetMachine> {
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
