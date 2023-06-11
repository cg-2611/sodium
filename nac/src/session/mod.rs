use std::cell::RefCell;

use crate::errors::{Diagnostic, DiagnosticHandler};

#[derive(Default)]
pub struct Session {
    diagnostic_handler: RefCell<DiagnosticHandler>,
}

impl Session {
    pub fn new() -> Self {
        Self {
            diagnostic_handler: Default::default(),
        }
    }

    pub fn report_diagnostic(&self, diagnostic: Diagnostic) {
        self.diagnostic_handler.borrow_mut().report(diagnostic);
    }

    pub fn emit_diagnostics(&self) {
        self.diagnostic_handler.borrow().emit();
    }

    pub fn has_errors(&self) -> bool {
        self.diagnostic_handler.borrow().has_errors()
    }

    pub fn error_count(&self) -> u32 {
        self.diagnostic_handler.borrow().error_count()
    }
}
