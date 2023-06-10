use crate::ast::expr::Literal;
use crate::ast::Identifier;
use crate::errors::{Diagnostic, DiagnosticLevel, Result};
use crate::parser::Parser;
use crate::source::Range;
use crate::token::TokenKind;

impl<'s> Parser<'s> {
    pub fn expected(&self, kind: TokenKind) -> Result<Range> {
        Err(self.expected_found(format!("expected {:?}", kind).as_str(), self.token.range))
    }

    pub fn expected_identifier(&self) -> Result<Identifier> {
        Err(self.error("expected identifier", self.token.range))
    }

    pub fn expected_integer_literal(&self) -> Result<Literal> {
        Err(self.error("expected integer literal", self.token.range))
    }

    pub fn error(&self, message: &str, range: Range) -> Diagnostic {
        self.expected_found(message, range)
    }

    fn expected_found(&self, message: &str, range: Range) -> Diagnostic {
        Diagnostic::new(
            DiagnosticLevel::Error,
            format!("{}, found {:?}", message, self.token.kind),
            range,
        )
    }
}
