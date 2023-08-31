use errors::{Diagnostic, DiagnosticHandler, ErrorOccurred, NACResult};
use range::Range;
use symbol::SymbolInterner;

#[derive(Default)]
pub struct Session {
    diagnostic_handler: DiagnosticHandler,
    symbol_interner: SymbolInterner,
}

impl Session {
    pub fn new() -> Self {
        Self {
            diagnostic_handler: DiagnosticHandler::new(),
            symbol_interner: SymbolInterner::new(),
        }
    }

    pub fn symbol_interner(&self) -> &SymbolInterner {
        &self.symbol_interner
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
