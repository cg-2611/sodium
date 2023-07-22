use crate::ast::ty::Type;
use crate::parser::{Parser, ParserResult};
use crate::token::TokenKind;

impl<'a> Parser<'a> {
    pub fn parse_ret_type(&mut self) -> ParserResult<'a, Box<Type>> {
        self.expect(TokenKind::Arrow)?;
        self.parse_type()
    }

    pub fn parse_type(&mut self) -> ParserResult<'a, Box<Type>> {
        let ident = self.parse_ident()?;
        Ok(Box::new(Type::new(ident)))
    }
}
