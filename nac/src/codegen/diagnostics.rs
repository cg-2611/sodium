use crate::codegen::CodeGen;
use crate::errors::{Diagnostic, ErrorOccurred};
use crate::source::Range;

pub type CodeGenError<'a> = Diagnostic<'a, ErrorOccurred>;
pub type CodeGenResult<'a, T> = Result<T, CodeGenError<'a>>;

impl<'a, 'ctx> CodeGen<'a, 'ctx> {
    pub fn codegen_error(&self, message: &str, range: Range) -> CodeGenError<'a> {
        self.session
            .create_ranged_error(String::from(message), range)
    }
}
