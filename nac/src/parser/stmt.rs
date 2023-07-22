use crate::ast::stmt::{Stmt, StmtKind};
use crate::parser::{Parser, ParserResult};
use crate::token::TokenKind;

impl<'a> Parser<'a> {
    pub fn parse_stmt(&mut self) -> ParserResult<'a, Stmt> {
        self.parse_expr_stmt()
    }

    pub fn parse_expr_stmt(&mut self) -> ParserResult<'a, Stmt> {
        let expr = self.parse_expr()?;
        let expr_range = expr.range;
        let semicolon = self.expect(TokenKind::Semicolon)?;
        Ok(Stmt::new(
            StmtKind::ExprStmt(expr),
            expr_range.to(semicolon),
        ))
    }
}
