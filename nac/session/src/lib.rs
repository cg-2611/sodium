use errors::{Diagnostic, DiagnosticHandler, ErrorOccurred, NACResult};
use range::Range;

#[derive(Default)]
pub struct Session {
    diagnostic_handler: DiagnosticHandler,
}

impl Session {
    pub fn new() -> Self {
        Self {
            diagnostic_handler: Default::default(),
        }
    }

    pub fn run_pass<'a, T>(
        &self,
        pass: impl FnOnce() -> Result<T, Diagnostic<'a, ErrorOccurred>>,
    ) -> NACResult<T> {
        let result = pass().map_err(|mut error| error.emit())?;
        self.has_errors()?;
        Ok(result)
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
