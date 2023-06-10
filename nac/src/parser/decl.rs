use crate::ast::decl::{Decl, DeclKind, FnDecl};
use crate::errors::Result;
use crate::parser::Parser;
use crate::token::{Keyword, TokenKind};

impl<'s> Parser<'s> {
    pub fn parse_decl(&mut self) -> Result<Option<Decl>> {
        match self.token.kind {
            TokenKind::EOF => Ok(None),
            TokenKind::Keyword(Keyword::Fn) => {
                let fn_decl = self.parse_fn_decl()?;
                let fn_decl_range = fn_decl.range;
                Ok(Some(Decl::new(
                    DeclKind::Fn(Box::new(fn_decl)),
                    fn_decl_range,
                )))
            }
            _ => Err(self.error("expected decl", self.token.range)),
        }
    }

    pub fn parse_fn_decl(&mut self) -> Result<FnDecl> {
        let fn_keyword = self.expect(TokenKind::Keyword(Keyword::Fn))?;

        let ident = self.parse_identifier()?;

        self.expect(TokenKind::LeftParen)?;
        self.expect(TokenKind::RightParen)?;

        let ret_type = self.parse_ret_type()?;
        let body = self.parse_block()?;
        let body_range = body.range;

        Ok(FnDecl::new(
            ident,
            ret_type,
            body,
            fn_keyword.to(body_range),
        ))
    }
}
