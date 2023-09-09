use std::cell::RefCell;

use range::Range;

use crate::diagnostic::Diagnostic;
use crate::{EmissionPending, ErrorOccurred};

#[derive(Default)]
pub struct DiagnosticHandler {
    inner: RefCell<DiagnosticHandlerInner>,
}

impl DiagnosticHandler {
    pub fn new() -> Self {
        Self {
            inner: RefCell::new(DiagnosticHandlerInner::new()),
        }
    }

    pub fn create_ranged_error(
        &self,
        message: String,
        file: String,
        range: Range,
    ) -> Diagnostic<'_, ErrorOccurred> {
        let mut error = self.create_error(message);
        error.set_location(file, range);
        error
    }

    pub fn create_error(&self, message: String) -> Diagnostic<'_, ErrorOccurred> {
        Diagnostic::new_error(self, message)
    }

    pub fn error_count(&self) -> usize {
        self.inner.borrow().error_count()
    }

    pub fn has_errors(&self) -> Option<ErrorOccurred> {
        self.inner.borrow().has_errors().then_some(ErrorOccurred)
    }

    pub fn emit_diagnostic(&self, diagnostic: &Diagnostic<impl EmissionPending>) -> ErrorOccurred {
        self.inner.borrow_mut().emit_diagnostic(diagnostic)
    }
}

#[derive(Default)]
pub struct DiagnosticHandlerInner {
    error_count: usize,
}

impl DiagnosticHandlerInner {
    pub fn new() -> Self {
        Self { error_count: 0 }
    }

    pub fn error_count(&self) -> usize {
        self.error_count
    }

    pub fn has_errors(&self) -> bool {
        self.error_count() > 0
    }

    pub fn emit_diagnostic(
        &mut self,
        diagnostic: &Diagnostic<impl EmissionPending>,
    ) -> ErrorOccurred {
        self.error_count += 1;
        eprintln!("{}", diagnostic);

        ErrorOccurred
    }
}
