use crate::Token;

pub struct TokenStream {
    tokens: Vec<Token>,
}

impl TokenStream {
    pub fn from(tokens: Vec<Token>) -> Self {
        Self { tokens }
    }

    pub fn vec(&self) -> &Vec<Token> {
        &self.tokens
    }
}
