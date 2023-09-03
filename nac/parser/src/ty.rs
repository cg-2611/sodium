use ast::Type;
use token::TokenKind;

use crate::{Parser, ParserResult};

impl<'a> Parser<'a> {
    pub fn parse_ret_type(&mut self) -> ParserResult<'a, Type> {
        self.expect(TokenKind::Arrow)?;
        self.parse_type()
    }

    pub fn parse_type(&mut self) -> ParserResult<'a, Type> {
        let ident = self.parse_ident()?;
        Ok(Type::new(ident))
    }
}
