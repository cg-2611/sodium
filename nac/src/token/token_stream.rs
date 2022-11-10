use super::Token;

pub struct TokenStream {
    tokens: Vec<Token>,
}

impl TokenStream {
    pub fn new(tokens: Vec<Token>) -> Self {
        Self { tokens }
    }

    pub fn vec(&self) -> &Vec<Token> {
        &self.tokens
    }
}
