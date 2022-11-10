use std::fmt::Display;

use super::source_range::SourceRange;

#[derive(Clone, Copy)]
pub struct SourceLocation {
    line: u32,
    column: u32,
}

impl SourceLocation {
    pub fn new(line: u32, column: u32) -> Self {
        Self { line, column }
    }

    pub fn line(&self) -> u32 {
        self.line
    }

    pub fn column(&self) -> u32 {
        self.column
    }

    pub fn to(&self, location: &Self) -> SourceRange {
        SourceRange::new(self.clone(), location.clone())
    }
}

impl Display for SourceLocation {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}:{}", self.line, self.column)
    }
}
