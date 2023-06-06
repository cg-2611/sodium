use crate::token::token_stream::TokenStream;
use crate::token::Token;

pub struct Cursor {
    tokens: TokenStream,
    index: usize,
}

impl Cursor {
    pub fn new(tokens: TokenStream) -> Self {
        Self { tokens, index: 0 }
    }

    pub fn advance(&mut self) -> Option<Token> {
        self.tokens.vec().get(self.index).map(|token| {
            self.index += 1;
            token.clone()
        })
    }

    pub fn peek(&self) -> Option<Token> {
        self.tokens.vec().get(self.index).cloned()
    }
}
