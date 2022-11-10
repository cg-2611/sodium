use crate::basic::{SourceCursor, SourceLocation};
use crate::token::token::{Keyword, Type};
use crate::token::{Token, TokenKind, TokenStream};

pub struct Lexer<'src> {
    cursor: SourceCursor<'src>,
    line: u32,
    column: u32,
}

impl<'src> Lexer<'src> {
    pub fn new(src: &'src str) -> Self {
        let source_cursor = SourceCursor::new(src);

        Self {
            cursor: source_cursor,
            line: 1,
            column: 1,
        }
    }

    pub fn tokenize(&mut self) -> TokenStream {
        let mut tokens: Vec<Token> = Vec::new();

        loop {
            let token = self.next_token();

            if *token.kind() == TokenKind::EOF {
                break;
            }

            tokens.push(token);
        }

        TokenStream::new(tokens)
    }

    pub fn next_token(&mut self) -> Token {
        self.skip_whitespace();

        let start = SourceLocation::new(self.line, self.column);

        let first_char = self.advance();

        let token_kind = match first_char {
            c if is_identifier_start(c) => self.tokenize_identifier(c),
            c if is_base_10_digit(c) => self.tokenize_integer_literal(c),
            '-' => match self.advance() {
                '>' => TokenKind::Arrow,
                _ => TokenKind::Unknown, // temporary until '-' is a valid token
            },
            '{' => TokenKind::LeftBrace,
            '}' => TokenKind::RightBrace,
            '(' => TokenKind::LeftParen,
            ')' => TokenKind::RightParen,
            ';' => TokenKind::Semicolon,
            '\0' => TokenKind::EOF,
            _ => TokenKind::Unknown,
        };

        self.make_token(token_kind, start)
    }

    fn make_token(&self, kind: TokenKind, start: SourceLocation) -> Token {
        let end = SourceLocation::new(self.line, self.column);
        Token::new(kind, start.to(&end))
    }

    fn advance(&mut self) -> char {
        self.column += 1;
        self.cursor.next().unwrap_or('\0')
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
            "func" => TokenKind::Keyword(Keyword::Func),
            "return" => TokenKind::Keyword(Keyword::Return),
            "int" => TokenKind::Type(Type::Int),
            _ => TokenKind::Identifier(identifier),
        }
    }

    fn tokenize_integer_literal(&mut self, c: char) -> TokenKind {
        let mut integer_literal_string = c.to_string();

        while is_base_10_digit(self.peek()) {
            let c = self.advance();
            integer_literal_string.push(c);
        }

        let integer_literal: i32 = integer_literal_string.parse().unwrap();

        TokenKind::IntegerLiteral(integer_literal)
    }
}

fn is_identifier_start(c: char) -> bool {
    c.is_alphabetic() || c == '_'
}

fn is_base_10_digit(c: char) -> bool {
    c.is_digit(10)
}
