use std::fmt::Display;

use super::source_location::SourceLocation;

pub struct SourceRange {
    start: SourceLocation,
    end: SourceLocation,
}

impl SourceRange {
    pub fn new(start: SourceLocation, end: SourceLocation) -> Self {
        Self { start, end }
    }

    pub fn start(&self) -> &SourceLocation {
        &self.start
    }

    pub fn end(&self) -> &SourceLocation {
        &self.end
    }
}

impl Display for SourceRange {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{} -> {}", self.start(), self.end())
    }
}
