use crate::errors::{Diagnostic, ErrorOccurred};

pub type LexerError<'a> = Diagnostic<'a, ErrorOccurred>;
pub type LexerResult<'a, T> = Result<T, LexerError<'a>>;
