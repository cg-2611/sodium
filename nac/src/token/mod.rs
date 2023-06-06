pub mod cursor;
pub mod token_stream;

use crate::source::Range;

#[derive(Debug, Clone, PartialEq)]
pub enum TokenKind {
    // keywords
    Keyword(Keyword),

    // literal tokens
    Identifier(String),
    IntegerLiteral(i32),

    // symbols
    Arrow,
    LeftBrace,
    RightBrace,
    LeftParen,
    RightParen,
    Semicolon,

    // misc
    EOF,
    Unknown(char),
}
#[derive(Debug, Clone, PartialEq)]
pub enum Keyword {
    Fn,
    Ret,
}

#[derive(Clone)]
pub struct Token {
    kind: TokenKind,
    range: Range,
}

impl Token {
    pub fn new(kind: TokenKind, range: Range) -> Self {
        Self { kind, range }
    }

    pub fn dummy() -> Self {
        Self {
            kind: TokenKind::EOF,
            range: Range::dummy(),
        }
    }

    pub fn kind(&self) -> &TokenKind {
        &self.kind
    }

    pub fn range(&self) -> &Range {
        &self.range
    }
}
