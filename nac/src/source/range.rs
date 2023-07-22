use std::fmt::Display;

#[derive(Debug, Copy, Clone)]
pub struct Location {
    line: u32,
    column: u32,
}

impl Location {
    pub fn new(line: u32, column: u32) -> Self {
        Self { line, column }
    }

    pub fn dummy() -> Self {
        Self { line: 0, column: 0 }
    }
}

#[derive(Debug, Copy, Clone)]
pub struct Range {
    start: Location,
    end: Location,
}

impl Range {
    pub fn new(start: Location, end: Location) -> Self {
        Self { start, end }
    }

    pub fn dummy() -> Self {
        Self {
            start: Location::dummy(),
            end: Location::dummy(),
        }
    }

    pub fn start(&self) -> &Location {
        &self.start
    }

    pub fn end(&self) -> &Location {
        &self.end
    }

    pub fn to(&self, end: Range) -> Self {
        Self {
            start: self.start,
            end: end.end,
        }
    }
}

impl Display for Location {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}:{}", self.line, self.column)
    }
}

impl Display for Range {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{} -> {}", self.start(), self.end())
    }
}
