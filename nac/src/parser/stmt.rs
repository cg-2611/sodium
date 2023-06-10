use crate::ast::stmt::{Stmt, StmtKind};
use crate::errors::Result;
use crate::parser::Parser;
use crate::token::TokenKind;

impl<'s> Parser<'s> {
    pub fn parse_stmt(&mut self) -> Result<Stmt> {
        self.parse_expr_stmt()
    }

    pub fn parse_expr_stmt(&mut self) -> Result<Stmt> {
        let expr = self.parse_expr()?;
        let expr_range = expr.range;
        let semicolon = self.expect(TokenKind::Semicolon)?;
        Ok(Stmt::new(
            StmtKind::ExprStmt(expr),
            expr_range.to(semicolon),
        ))
    }
}
