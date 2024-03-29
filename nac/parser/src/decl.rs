use ast::{Decl, DeclKind, FnDecl};
use token::{Keyword, TokenKind};

use crate::{Parser, ParserResult};

impl<'a> Parser<'a> {
    pub fn parse_decl(&mut self) -> ParserResult<'a, Option<Decl>> {
        match self.token.kind {
            TokenKind::EOF => Ok(None),
            TokenKind::Keyword(Keyword::Fn) => {
                let fn_decl = self.parse_fn_decl()?;
                let fn_decl_range = fn_decl.range;
                let decl = Decl::new(DeclKind::Fn(fn_decl), fn_decl_range);
                Ok(Some(decl))
            }
            _ => Err(self.parser_error("expected decl", self.token.range)),
        }
    }

    pub fn parse_fn_decl(&mut self) -> ParserResult<'a, FnDecl> {
        let fn_keyword = self.expect(TokenKind::Keyword(Keyword::Fn))?;

        let ident = self.parse_ident()?;

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
