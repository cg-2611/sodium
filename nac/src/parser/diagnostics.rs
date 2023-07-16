use crate::ast::expr::Literal;
use crate::ast::Identifier;
use crate::errors::{Diagnostic, DiagnosticLevel, Result};
use crate::parser::Parser;
use crate::source::Range;
use crate::token::{Keyword, TokenKind};

impl<'s> Parser<'s> {
    pub fn report_diagnostic(
        &mut self,
        diagnostic: Diagnostic,
        recover: Option<impl Fn(&mut Parser<'s>)>,
    ) {
        self.session.report_diagnostic(diagnostic);
        if let Some(f) = recover {
            f(self)
        };
    }

    pub fn expected_token(&self, kind: TokenKind) -> Result<Range> {
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
        Diagnostic::ranged(
            DiagnosticLevel::Error,
            format!("{}, found {:?}", message, self.token.kind),
            range,
        )
    }

    pub fn recover_decl(&mut self) {
        self.recover(&[TokenKind::Keyword(Keyword::Fn)]);
    }

    pub fn recover_expr(&mut self) {
        self.recover(&[TokenKind::Semicolon]);
        self.advance();
    }

    pub fn recover_stmt(&mut self) {
        self.recover(&[TokenKind::LeftBrace, TokenKind::Semicolon]);
        if self.token.kind == TokenKind::Semicolon {
            self.advance();
        }
    }

    pub fn recover(&mut self, delimiters: &[TokenKind]) {
        while !delimiters.contains(&self.token.kind) && self.token.kind != TokenKind::EOF {
            self.advance();
        }
    }
}
