use range::Range;
use std::fmt::{Debug, Display};
use std::marker::PhantomData;

use crate::{DiagnosticHandler, EmissionPending, ErrorOccurred};

pub enum DiagnosticState<'a> {
    EmissionPending(&'a DiagnosticHandler),
    Emitted,
}

pub struct Diagnostic<'a, E: EmissionPending> {
    pub state: DiagnosticState<'a>,
    data: Box<DiagnosticData>,
    _marker: PhantomData<E>,
}

impl<'a> Diagnostic<'a, ErrorOccurred> {
    pub fn new_error(handler: &'a DiagnosticHandler, message: String) -> Self {
        Self {
            state: DiagnosticState::EmissionPending(handler),
            data: Box::new(DiagnosticData::new_error(message)),
            _marker: PhantomData,
        }
    }

    pub fn set_range(&mut self, range: Range) {
        self.data.set_range(range);
    }
}

impl<'a, E: EmissionPending> Diagnostic<'a, E> {
    pub fn emit(&mut self) -> E {
        E::emit_pending_diagnostic(self)
    }
}

impl<'a, E: EmissionPending> Display for Diagnostic<'a, E> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.data)
    }
}

impl<E: EmissionPending> Debug for Diagnostic<'_, E> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        self.data.fmt(f)
    }
}

impl<E: EmissionPending> Drop for Diagnostic<'_, E> {
    fn drop(&mut self) {
        match self.state {
            DiagnosticState::Emitted => {}
            DiagnosticState::EmissionPending(handler) => {
                handler.emit_diagnostic(self);
                panic!("error was constructed but not emitted");
            }
        }
    }
}

#[derive(Debug)]
enum DiagnosticLevel {
    Error,
}

struct DiagnosticData {
    level: DiagnosticLevel,
    message: String,
    range: Option<Range>,
}

impl DiagnosticData {
    fn new_error(message: String) -> Self {
        Self {
            level: DiagnosticLevel::Error,
            message,
            range: None,
        }
    }

    fn set_range(&mut self, range: Range) {
        self.range = Some(range);
    }
}

impl Display for DiagnosticData {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match &self.range {
            Some(range) => write!(f, "{:?}: {}: {}", self.level, range, self.message),
            None => write!(f, "{:?}: {}", self.level, self.message),
        }
    }
}
