use llvm::Context;
use session::Session;
use ty::TypeContext;

pub struct CompilerContext<'ctx> {
    session: &'ctx Session,
    llvm_context: Context,
    type_context: TypeContext,
}

impl<'ctx> CompilerContext<'ctx> {
    pub fn new(session: &'ctx Session) -> Self {
        Self {
            session,
            llvm_context: Context::create(),
            type_context: TypeContext::new(),
        }
    }

    pub fn session(&self) -> &Session {
        self.session
    }

    pub fn llvm_context(&self) -> &Context {
        &self.llvm_context
    }

    pub fn type_context(&self) -> &TypeContext {
        &self.type_context
    }
}
