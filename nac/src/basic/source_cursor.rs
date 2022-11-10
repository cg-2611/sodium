use std::str::Chars;

pub struct SourceCursor<'src> {
    chars: Chars<'src>,
}

impl<'src> SourceCursor<'src> {
    pub fn new(src: &'src str) -> Self {
        Self { chars: src.chars() }
    }

    pub fn next(&mut self) -> Option<char> {
        self.chars.next()
    }

    pub fn peek(&self) -> Option<char> {
        self.chars.clone().next()
    }
}
