use crate::ast::stmt::{Stmt, StmtKind};
use crate::errors::{Diagnostic, DiagnosticLevel, Result};
use crate::parser::Parser;
use crate::token::{Keyword, TokenKind};

impl Parser {
    pub fn parse_stmt(&mut self) -> Result<Stmt> {
        match self.token.kind() {
            TokenKind::Keyword(Keyword::Ret) => {
                let ret_expr = self.parse_ret_expr()?;

                if !self.expect(&TokenKind::Semicolon) {
                    return Err(Diagnostic::new(
                        DiagnosticLevel::Error,
                        format!("expected ;, found {:?}", self.token.kind()),
                        ret_expr.range(),
                    ));
                }

                let start = *ret_expr.range().start();
                let semicolon = *self.token.range().start();

                Ok(Stmt::new(StmtKind::RetExpr(ret_expr), start.to(&semicolon)))
            }
            _ => Err(Diagnostic::new(
                DiagnosticLevel::Error,
                format!("expected stmt, found {:?}", self.token.kind()),
                *self.token.range(),
            )),
        }
    }
}
