pub use diagnostic::{Diagnostic, DiagnosticLevel};
pub use diagnostic_handler::DiagnosticHandler;

pub mod diagnostic;
pub mod diagnostic_handler;

pub type Result<T> = std::result::Result<T, Diagnostic>;
