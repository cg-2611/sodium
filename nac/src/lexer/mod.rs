use crate::errors::Result;
use crate::session::Session;
use crate::source::{Cursor, Location, Range};
use crate::token::token_stream::TokenStream;
use crate::token::{Keyword, Token, TokenKind};

#[cfg(test)]
mod tests;

pub mod diagnostics;

pub struct Lexer<'src> {
    cursor: Cursor<'src>,
    line: u32,
    column: u32,
}

impl<'src> Lexer<'src> {
    pub fn new(src: &'src str) -> Self {
        Self {
            cursor: Cursor::new(src),
            line: 1,
            column: 1,
        }
    }

    pub fn tokenize(session: &Session, src: &'src str) -> Result<TokenStream> {
        let mut tokens: Vec<Token> = Vec::new();
        let mut lexer = Lexer::new(src);

        loop {
            let token = lexer.next_token();

            match token.kind {
                TokenKind::EOF => {
                    tokens.push(token);
                    break;
                }
                TokenKind::Unknown(c) => {
                    session.report_diagnostic(lexer.unrecognised_token_error(c, token.range));
                    tokens.push(token);
                }
                _ => tokens.push(token),
            }
        }

        Ok(TokenStream::from(tokens))
    }

    pub fn next_token(&mut self) -> Token {
        self.skip_whitespace();

        let start = Location::new(self.line, self.column);
        let first_char = self.advance();
        let token_kind = match first_char {
            c if is_identifier_start(c) => self.tokenize_identifier(c),
            c if is_base_10_digit(c) => self.tokenize_integer_literal(c),
            '-' => match self.advance() {
                '>' => TokenKind::Arrow,
                _ => TokenKind::Unknown(first_char), // temporary until '-' is a valid token
            },
            '{' => TokenKind::LeftBrace,
            '}' => TokenKind::RightBrace,
            '(' => TokenKind::LeftParen,
            ')' => TokenKind::RightParen,
            ';' => TokenKind::Semicolon,
            '\0' => TokenKind::EOF,
            _ => TokenKind::Unknown(first_char),
        };

        self.make_token(token_kind, start)
    }

    fn make_token(&self, kind: TokenKind, start: Location) -> Token {
        Token::new(
            kind,
            Range::new(start, Location::new(self.line, self.column)),
        )
    }

    fn advance(&mut self) -> char {
        self.cursor.advance().map_or('\0', |c| {
            self.column += 1;
            c
        })
    }

    fn peek(&self) -> char {
        self.cursor.peek().unwrap_or('\0')
    }

    fn skip_whitespace(&mut self) {
        while self.peek().is_whitespace() {
            if self.peek() == '\n' {
                self.line += 1;
                self.column = 0;
            }

            self.advance();
        }
    }

    fn tokenize_identifier(&mut self, c: char) -> TokenKind {
        let mut identifier = c.to_string();

        while is_identifier_start(self.peek()) || is_base_10_digit(self.peek()) {
            let c = self.advance();
            identifier.push(c);
        }

        match identifier.as_str() {
            "fn" => TokenKind::Keyword(Keyword::Fn),
            "ret" => TokenKind::Keyword(Keyword::Ret),
            _ => TokenKind::Identifier(identifier),
        }
    }

    fn tokenize_integer_literal(&mut self, c: char) -> TokenKind {
        let mut integer_literal = c.to_string();
        while is_base_10_digit(self.peek()) {
            let c = self.advance();
            integer_literal.push(c);
        }

        let integer_literal: i32 = integer_literal.parse().unwrap();
        TokenKind::IntegerLiteral(integer_literal)
    }
}

fn is_identifier_start(c: char) -> bool {
    c.is_alphabetic() || c == '_'
}

fn is_base_10_digit(c: char) -> bool {
    c.is_ascii_digit()
}
