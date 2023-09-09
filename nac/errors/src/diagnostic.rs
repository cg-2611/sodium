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

    pub fn set_location(&mut self, file: String, range: Range) {
        self.data.set_location(file, range);
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

pub struct DiagnosticLocation {
    file: String,
    range: Range,
}

impl DiagnosticLocation {
    pub fn new(file: String, range: Range) -> Self {
        Self { file, range }
    }

    pub fn file(&self) -> &String {
        &self.file
    }

    pub fn range(&self) -> &Range {
        &self.range
    }
}

#[derive(Debug)]
enum DiagnosticLevel {
    Error,
}

impl DiagnosticLevel {
    pub fn to_str(&self) -> &str {
        match self {
            DiagnosticLevel::Error => "error",
        }
    }
}

struct DiagnosticData {
    location: Option<DiagnosticLocation>,
    level: DiagnosticLevel,
    message: String,
}

impl DiagnosticData {
    fn new_error(message: String) -> Self {
        Self {
            location: None,
            level: DiagnosticLevel::Error,
            message,
        }
    }

    fn set_location(&mut self, file: String, range: Range) {
        self.location = Some(DiagnosticLocation::new(file, range));
    }
}

impl Display for DiagnosticData {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match &self.location {
            Some(diagnostic_location) => write!(
                f,
                "{}:{}: {}: {}",
                diagnostic_location.file,
                diagnostic_location.range.start(),
                self.level.to_str(),
                self.message
            ),
            None => write!(f, "{}: {}", self.level.to_str(), self.message),
        }
    }
}
