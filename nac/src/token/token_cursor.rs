use super::{Token, TokenStream};

pub struct TokenCursor<'ts> {
    tokens: &'ts TokenStream,
    index: usize,
}

impl<'ts> TokenCursor<'ts> {
    pub fn new(tokens: &'ts TokenStream) -> Self {
        Self { tokens, index: 0 }
    }

    pub fn next(&mut self) -> Option<&'ts Token> {
        self.tokens.vec().get(self.index).map(|token| {
            self.index += 1;
            token
        })
    }
}
