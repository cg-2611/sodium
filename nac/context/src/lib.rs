use llvm::Context;
use session::Session;

pub struct CompilerContext<'ctx> {
    session: &'ctx Session,
    llvm_context: Context,
}

impl<'ctx> CompilerContext<'ctx> {
    pub fn new(session: &'ctx Session) -> Self {
        Self {
            session,
            llvm_context: Context::create(),
        }
    }

    pub fn session(&self) -> &Session {
        self.session
    }

    pub fn llvm_context(&self) -> &Context {
        &self.llvm_context
    }
}
