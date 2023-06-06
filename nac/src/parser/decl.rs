use crate::ast::decl::{Decl, DeclKind, FnDecl};
use crate::errors::{Diagnostic, DiagnosticLevel, Result};
use crate::parser::Parser;
use crate::token::{Keyword, TokenKind};

impl Parser {
    pub fn parse_decl(&mut self) -> Result<Decl> {
        match self.token.kind() {
            TokenKind::Keyword(Keyword::Fn) => {
                let fn_decl = self.parse_fn_decl()?;
                Ok(Decl::new(DeclKind::Fn(fn_decl), *self.token.range()))
            }
            _ => Err(Diagnostic::new(
                DiagnosticLevel::Error,
                format!("expected decl, found {:?}", self.token.kind()),
                *self.token.range(),
            )),
        }
    }

    pub fn parse_fn_decl(&mut self) -> Result<FnDecl> {
        let fn_keyword = *self.token.range().start();

        if !self.expect(&TokenKind::Keyword(Keyword::Fn)) {
            return Err(Diagnostic::new(
                DiagnosticLevel::Error,
                format!("expected fn keyword, found {:?}", self.token.kind()),
                fn_keyword.to(self.token.range().end()),
            ));
        }

        let name = self.parse_identifier()?;

        if !self.expect(&TokenKind::LeftParen) {
            return Err(Diagnostic::new(
                DiagnosticLevel::Error,
                format!("expected (, found {:?}", self.token.kind()),
                fn_keyword.to(self.token.range().end()),
            ));
        }

        if !self.expect(&TokenKind::RightParen) {
            return Err(Diagnostic::new(
                DiagnosticLevel::Error,
                format!("expected ), found {:?}", self.token.kind()),
                fn_keyword.to(self.token.range().end()),
            ));
        }

        let ret_type = self.parse_ret_type()?;

        let body = self.parse_block()?;

        Ok(FnDecl::new(name, ret_type, body))
    }
}
