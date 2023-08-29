use errors::{Diagnostic, ErrorOccurred};

pub type SemaError<'cx> = Diagnostic<'cx, ErrorOccurred>;
pub type SemaResult<'cx, T> = Result<T, SemaError<'cx>>;
