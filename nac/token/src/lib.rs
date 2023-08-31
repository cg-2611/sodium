use range::Range;
use symbol::Ident;

pub use self::cursor::Cursor;
pub use self::token_stream::TokenStream;

pub mod cursor;
pub mod token_stream;

#[derive(Debug, Copy, Clone, Eq, PartialEq)]
pub enum TokenKind {
    // keywords
    Keyword(Keyword),

    // literal tokens
    Identifier(Ident),
    IntegerLiteral(i32),

    // symbols
    Arrow,      // "->"
    LeftBrace,  // "{"
    RightBrace, // "}"
    LeftParen,  // "("
    RightParen, // ")"
    Semicolon,  // ";"

    // misc
    EOF,
    Unknown(char),
}
#[derive(Debug, Copy, Clone, Eq, PartialEq)]
pub enum Keyword {
    Fn,
    Ret,
}

#[derive(Debug, Copy, Clone)]
pub struct Token {
    pub kind: TokenKind,
    pub range: Range,
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
}
