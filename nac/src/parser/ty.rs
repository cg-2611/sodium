use crate::ast::ty::Type;
use crate::errors::Result;
use crate::parser::Parser;
use crate::token::TokenKind;

impl<'s> Parser<'s> {
    pub fn parse_ret_type(&mut self) -> Result<Box<Type>> {
        self.expect(TokenKind::Arrow)?;
        self.parse_type()
    }

    pub fn parse_type(&mut self) -> Result<Box<Type>> {
        let ident = self.parse_identifier()?;
        Ok(Box::new(Type::new(ident)))
    }
}
