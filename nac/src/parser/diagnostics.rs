use crate::ast::expr::Literal;
use crate::ast::Identifier;
use crate::errors::{Diagnostic, ErrorOccurred};
use crate::parser::Parser;
use crate::source::Range;
use crate::token::{Keyword, TokenKind};

pub type ParserError<'a> = Diagnostic<'a, ErrorOccurred>;
pub type ParserResult<'a, T> = Result<T, ParserError<'a>>;

impl<'a> Parser<'a> {
    pub fn parser_error(&self, message: &str, range: Range) -> ParserError<'a> {
        self.session
            .create_ranged_error(String::from(message), range)
    }

    pub fn error_expected_token(&self, kind: TokenKind) -> ParserResult<'a, Range> {
        Err(self.error_expected_found(format!("expected {:?}", kind).as_str(), self.token.range))
    }

    pub fn expected_identifier(&self) -> ParserResult<'a, Identifier> {
        Err(self.error_expected_found("expected identifier", self.token.range))
    }

    pub fn expected_integer_literal(&self) -> ParserResult<'a, Literal> {
        Err(self.error_expected_found("expected integer literal", self.token.range))
    }

    fn error_expected_found(&self, message: &str, range: Range) -> ParserError<'a> {
        let message = format!("{}, found {:?}", message, self.token.kind);
        self.session.create_ranged_error(message, range)
    }

    pub fn recover_decl(&mut self) {
        self.recover(&[TokenKind::Keyword(Keyword::Fn)]);
    }

    pub fn recover_expr(&mut self) {
        self.recover(&[TokenKind::Semicolon]);
        self.advance();
    }

    pub fn recover_stmt(&mut self) {
        self.recover(&[
            TokenKind::LeftBrace,
            TokenKind::RightBrace,
            TokenKind::Semicolon,
        ]);
        if self.token.kind == TokenKind::Semicolon {
            self.advance();
        }
    }

    pub fn recover(&mut self, delimiters: &[TokenKind]) {
        while !delimiters.contains(&self.token.kind) && self.token.kind != TokenKind::EOF {
            self.advance();
        }
    }
}
