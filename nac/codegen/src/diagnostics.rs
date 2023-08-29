use errors::{Diagnostic, ErrorOccurred};
use range::Range;

use crate::CodeGen;

pub type CodeGenError<'cx> = Diagnostic<'cx, ErrorOccurred>;
pub type CodeGenResult<'cx, T> = Result<T, CodeGenError<'cx>>;

impl<'cx> CodeGen<'cx> {
    pub fn codegen_error(&self, message: &str, range: Range) -> CodeGenError<'cx> {
        self.sess.create_ranged_error(String::from(message), range)
    }
}
