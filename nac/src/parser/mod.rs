use crate::ast::decl::Decl;
use crate::ast::{Identifier, SourceFile, AST};
use crate::errors::{Diagnostic, DiagnosticLevel, Result};
use crate::session::Session;
use crate::source::Range;
use crate::token::cursor::Cursor;
use crate::token::token_stream::TokenStream;
use crate::token::{Token, TokenKind};

pub mod decl;
pub mod diagnostics;
pub mod expr;
pub mod stmt;
pub mod ty;

pub struct Parser {
    cursor: Cursor,
    token: Token,
}

impl Parser {
    fn new(token_stream: TokenStream) -> Self {
        Self {
            cursor: Cursor::new(token_stream),
            token: Token::dummy(),
        }
    }

    pub fn parse(session: &Session, token_stream: TokenStream) -> Result<AST> {
        let mut parser = Parser::new(token_stream);
        let root = parser.parse_source_file()?;
        Ok(AST::new(root))
    }

    pub fn parse_source_file(&mut self) -> Result<SourceFile> {
        let mut decls: Vec<Decl> = Vec::new();

        loop {
            self.advance();
            match self.token.kind() {
                TokenKind::EOF => break,
                _ => {
                    let decl = self.parse_decl()?;
                    decls.push(decl);
                }
            }
        }

        Ok(SourceFile::new(decls, Range::dummy()))
    }

    pub fn parse_identifier(&mut self) -> Result<Identifier> {
        let kind = self.token.kind().clone();
        match kind {
            TokenKind::Identifier(value) => {
                self.advance();
                Ok(Identifier::new(value, Range::dummy()))
            }
            _ => Err(Diagnostic::new(
                DiagnosticLevel::Error,
                format!("expected identifier, found {:?}", self.token.kind()),
                *self.token.range(),
            )),
        }
    }

    fn advance(&mut self) {
        self.token = self.cursor.advance().unwrap_or_else(Token::dummy);
    }

    fn expect(&mut self, kind: &TokenKind) -> bool {
        let present = self.token.kind() == kind;
        if present {
            self.advance()
        }

        present
    }
}
