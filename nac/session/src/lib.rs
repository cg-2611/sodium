use errors::{Diagnostic, DiagnosticHandler, ErrorOccurred, NACResult};
use range::Range;

pub struct Session {
    diagnostic_handler: DiagnosticHandler,
}

impl Session {
    pub fn new() -> Self {
        Self {
            diagnostic_handler: DiagnosticHandler::default(),
        }
    }

    pub fn create_ranged_error(
        &self,
        message: String,
        range: Range,
    ) -> Diagnostic<'_, ErrorOccurred> {
        self.diagnostic_handler.create_ranged_error(message, range)
    }

    pub fn create_error(&self, message: String) -> Diagnostic<'_, ErrorOccurred> {
        self.diagnostic_handler.create_error(message)
    }

    pub fn error_count(&self) -> usize {
        self.diagnostic_handler.error_count()
    }

    pub fn has_errors(&self) -> NACResult<()> {
        self.diagnostic_handler.has_errors().map_or(Ok(()), Err)
    }
}

impl Default for Session {
    fn default() -> Self {
        Self::new()
    }
}
