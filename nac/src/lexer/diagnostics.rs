use crate::errors::{Diagnostic, DiagnosticLevel};
use crate::lexer::Lexer;
use crate::source::Range;

impl<'src> Lexer<'src> {
    pub fn unrecognised_token_error(&self, c: char, range: Range) -> Diagnostic {
        Diagnostic::ranged(
            DiagnosticLevel::Error,
            format!("unrecognised token '{}'", c),
            range,
        )
    }
}
