use ast::decl::Decl;
use ast::{Identifier, SourceFile, AST};
use range::Range;
use session::Session;
use token::{Cursor, Token, TokenKind, TokenStream};

pub use self::diagnostics::{ParserError, ParserResult};

#[cfg(test)]
mod tests;

pub mod decl;
pub mod diagnostics;
pub mod expr;
pub mod stmt;
pub mod ty;

pub struct Parser<'a> {
    session: &'a Session,
    cursor: Cursor,
    token: Token,
}

impl<'a> Parser<'a> {
    pub fn new(session: &'a Session, token_stream: TokenStream) -> Self {
        let mut parser = Self {
            session,
            cursor: Cursor::new(token_stream),
            token: Token::dummy(),
        };

        parser.advance();
        parser
    }

    pub fn parse(session: &'a Session, token_stream: TokenStream) -> ParserResult<'a, AST> {
        let mut parser = Parser::new(session, token_stream);
        let root = parser.parse_source_file()?;
        Ok(AST::new(root))
    }

    pub fn parse_source_file(&mut self) -> ParserResult<'a, SourceFile> {
        let start = self.token.range;
        let mut decls: Vec<Box<Decl>> = Vec::new();

        loop {
            match self.parse_decl() {
                Ok(Some(decl)) => decls.push(Box::new(decl)),
                Ok(None) => break,
                Err(mut diagnostic) => {
                    diagnostic.emit();
                    self.recover_decl();
                }
            }
        }

        Ok(SourceFile::new(decls, start.to(self.token.range)))
    }

    pub fn parse_ident(&mut self) -> ParserResult<'a, Identifier> {
        let ident = self.expect_ident()?;
        self.advance();

        Ok(ident)
    }

    fn advance(&mut self) {
        self.token = self.cursor.advance().unwrap_or_else(Token::dummy);
    }

    fn expect(&mut self, kind: TokenKind) -> ParserResult<'a, Range> {
        if self.token.kind == kind {
            let range = self.token.range;
            self.advance();
            Ok(range)
        } else {
            self.error_expected_token(kind)
        }
    }

    fn expect_ident(&self) -> ParserResult<'a, Identifier> {
        if let TokenKind::Identifier(value) = &self.token.kind {
            Ok(Identifier::new(value.to_string(), self.token.range))
        } else {
            self.expected_identifier()
        }
    }
}
