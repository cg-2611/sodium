use crate::errors::{Diagnostic, DiagnosticLevel};
use crate::source::Range;

pub fn unrecognised_token_error(c: &char, range: Range) -> Diagnostic {
    Diagnostic::new(
        DiagnosticLevel::Error,
        format!("unrecognised token '{}'", c),
        range,
    )
}
