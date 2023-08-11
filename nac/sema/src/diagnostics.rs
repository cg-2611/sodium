use errors::{Diagnostic, ErrorOccurred};
use range::Range;

use crate::Sema;

pub type SemaError<'ctx> = Diagnostic<'ctx, ErrorOccurred>;
pub type SemaResult<'ctx, T> = Result<T, SemaError<'ctx>>;

impl<'ctx> Sema<'ctx> {
    pub fn sema_error(&self, message: &str, range: Range) -> SemaError<'ctx> {
        self.context
            .session()
            .create_ranged_error(String::from(message), range)
    }
}
