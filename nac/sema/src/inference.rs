use context::CompilerContext;
use ty::Type;

use crate::SemaResult;

pub struct TypeInference<'ctx> {
    context: &'ctx CompilerContext<'ctx>,
}

impl<'ctx> TypeInference<'ctx> {
    pub fn new(context: &'ctx CompilerContext) -> Self {
        Self { context }
    }

    pub fn ast_type_to_type(&self, ast_type: &ast::Type) -> SemaResult<'ctx, Type> {
        match ast_type.ident.value.as_str() {
            "i32" => Ok(self.context.type_context().i32()),
            _ => {
                let message = format!("unknown type `{:?}`", ast_type.ident.value);
                Err(self
                    .context
                    .session()
                    .create_ranged_error(message, ast_type.ident.range))
            }
        }
    }
}
