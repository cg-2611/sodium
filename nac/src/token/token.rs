use crate::basic::source_range::SourceRange;

#[derive(Debug, PartialEq, Eq)]
pub enum TokenKind {
    // reserved words
    Keyword(Keyword),
    Type(Type),

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
    Unknown,
}

#[derive(Debug, PartialEq, Eq)]
pub enum Keyword {
    Func,
    Return,
}

#[derive(Debug, PartialEq, Eq)]
pub enum Type {
    Int,
}

pub struct Token {
    kind: TokenKind,
    range: SourceRange,
}

impl Token {
    pub fn new(kind: TokenKind, range: SourceRange) -> Self {
        Self { kind, range }
    }

    pub fn kind(&self) -> &TokenKind {
        &self.kind
    }

    pub fn range(&self) -> &SourceRange {
        &self.range
    }
}
