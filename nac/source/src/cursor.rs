use std::str::Chars;

pub struct Cursor<'src> {
    chars: Chars<'src>,
}

impl<'src> Cursor<'src> {
    pub fn new(src: &'src str) -> Self {
        Self { chars: src.chars() }
    }

    pub fn advance(&mut self) -> Option<char> {
        self.chars.next()
    }

    pub fn peek(&self) -> Option<char> {
        self.chars.clone().next()
    }
}
