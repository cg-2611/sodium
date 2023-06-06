use std::fmt::Display;

use crate::source::Range;

#[derive(Debug)]
pub enum DiagnosticLevel {
    Error,
    Fatal,
}

#[derive(Debug)]
pub struct Diagnostic {
    level: DiagnosticLevel,
    message: String,
    range: Option<Range>,
}

impl Diagnostic {
    pub fn new(level: DiagnosticLevel, message: String, range: Range) -> Self {
        Self {
            level,
            message,
            range: Some(range),
        }
    }

    pub fn without_range(level: DiagnosticLevel, message: String) -> Self {
        Self {
            level,
            message,
            range: None,
        }
    }

    pub fn level(&self) -> &DiagnosticLevel {
        &self.level
    }

    pub fn message(&self) -> &String {
        &self.message
    }

    pub fn range(&self) -> &Option<Range> {
        &self.range
    }
}

impl Display for Diagnostic {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match &self.range {
            Some(range) => write!(f, "{}: {}", range, self.message),
            None => write!(f, "{}", self.message),
        }
    }
}
