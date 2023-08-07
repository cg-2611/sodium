pub use diagnostic::Diagnostic;
pub use diagnostic_handler::DiagnosticHandler;

use crate::diagnostic::DiagnosticState;

pub mod diagnostic;
pub mod diagnostic_handler;

pub type NACResult<T> = Result<T, ErrorOccurred>;

pub struct ErrorOccurred;

impl EmissionPending for ErrorOccurred {
    fn emit_pending_diagnostic(diagnostic: &mut Diagnostic<'_, Self>) -> Self {
        match diagnostic.state {
            DiagnosticState::EmissionPending(handler) => {
                diagnostic.state = DiagnosticState::Emitted;
                handler.emit_diagnostic(diagnostic)
            }
            DiagnosticState::Emitted => ErrorOccurred,
        }
    }
}

pub trait EmissionPending: Sized {
    fn emit_pending_diagnostic(diagnostic: &mut Diagnostic<'_, Self>) -> Self;
}
