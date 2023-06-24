use crate::ast::decl::Decl;
use crate::ast::{Identifier, SourceFile, AST};
use crate::errors::Result;
use crate::session::Session;
use crate::source::Range;
use crate::token::cursor::Cursor;
use crate::token::token_stream::TokenStream;
use crate::token::{Token, TokenKind};

#[cfg(test)]
mod tests;

pub mod decl;
pub mod diagnostics;
pub mod expr;
pub mod stmt;
pub mod ty;

pub struct Parser<'s> {
    session: &'s Session,
    cursor: Cursor,
    token: Token,
}

impl<'s> Parser<'s> {
    pub fn new(session: &'s Session, token_stream: TokenStream) -> Self {
        let mut parser = Self {
            session,
            cursor: Cursor::new(token_stream),
            token: Token::dummy(),
        };

        parser.advance();
        parser
    }

    pub fn parse(session: &'s Session, token_stream: TokenStream) -> Result<AST> {
        let mut parser = Parser::new(session, token_stream);
        let root = parser.parse_source_file()?;
        Ok(AST::new(root))
    }

    pub fn parse_source_file(&mut self) -> Result<SourceFile> {
        let start = self.token.range;
        let mut decls: Vec<Box<Decl>> = Vec::new();

        loop {
            match self.parse_decl() {
                Ok(Some(decl)) => decls.push(Box::new(decl)),
                Ok(None) => break,
                Err(diagnostic) => {
                    self.report_diagnostic(diagnostic, Some(Parser::recover_decl));
                }
            }
        }

        Ok(SourceFile::new(decls, start.to(self.token.range)))
    }

    pub fn parse_identifier(&mut self) -> Result<Identifier> {
        let ident = self.expect_ident()?;
        self.advance();

        Ok(ident)
    }

    fn advance(&mut self) {
        self.token = self.cursor.advance().unwrap_or_else(Token::dummy);
    }

    fn expect(&mut self, kind: TokenKind) -> Result<Range> {
        if self.token.kind == kind {
            let range = self.token.range;
            self.advance();
            Ok(range)
        } else {
            self.expected_token(kind)
        }
    }

    fn expect_ident(&self) -> Result<Identifier> {
        if let TokenKind::Identifier(value) = &self.token.kind {
            Ok(Identifier::new(String::from(value), self.token.range))
        } else {
            self.expected_identifier()
        }
    }
}
