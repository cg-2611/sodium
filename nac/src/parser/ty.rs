use crate::ast::ty::Type;
use crate::errors::{Diagnostic, DiagnosticLevel, Result};
use crate::parser::Parser;
use crate::token::TokenKind;

impl Parser {
    pub fn parse_ret_type(&mut self) -> Result<Type> {
        if !self.expect(&TokenKind::Arrow) {
            return Err(Diagnostic::new(
                DiagnosticLevel::Error,
                format!("expected ->, found {:?}", self.token.kind()),
                *self.token.range(),
            ));
        }

        self.parse_type()
    }

    pub fn parse_type(&mut self) -> Result<Type> {
        let identifier = self.parse_identifier()?;
        Ok(Type::new(identifier))
    }
}
