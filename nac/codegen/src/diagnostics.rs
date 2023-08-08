use errors::{Diagnostic, ErrorOccurred};
use range::Range;

use crate::CodeGen;

pub type CodeGenError<'ctx> = Diagnostic<'ctx, ErrorOccurred>;
pub type CodeGenResult<'ctx, T> = Result<T, CodeGenError<'ctx>>;

impl<'ctx> CodeGen<'ctx> {
    pub fn codegen_error(&self, message: &str, range: Range) -> CodeGenError<'ctx> {
        self.context
            .session()
            .create_ranged_error(String::from(message), range)
    }
}
