use crate::errors::{Diagnostic, DiagnosticLevel};
use crate::llvm::LLVMString;
use crate::source::Range;

impl LLVMString {
    pub fn to_error(self) -> Diagnostic {
        Diagnostic::new(DiagnosticLevel::Error, self.as_string())
    }

    pub fn to_error_ranged(self, range: Range) -> Diagnostic {
        Diagnostic::ranged(DiagnosticLevel::Error, self.as_string(), range)
    }

    pub fn to_fatal(self) -> Diagnostic {
        Diagnostic::new(DiagnosticLevel::Fatal, self.as_string())
    }

    pub fn to_fatal_ranged(self, range: Range) -> Diagnostic {
        Diagnostic::ranged(DiagnosticLevel::Fatal, self.as_string(), range)
    }
}
