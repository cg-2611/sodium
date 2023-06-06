use crate::ast::expr::{Block, Expr, ExprKind, Literal, LiteralKind, RetExpr};
use crate::ast::stmt::Stmt;
use crate::errors::{Diagnostic, DiagnosticLevel, Result};
use crate::parser::Parser;
use crate::source::Range;
use crate::token::{Keyword, TokenKind};

impl Parser {
    pub fn parse_expr(&mut self) -> Result<Expr> {
        match self.token.kind() {
            TokenKind::LeftBrace => {
                let block = self.parse_block()?;
                Ok(Expr::new(ExprKind::Block(block), *self.token.range()))
            }
            TokenKind::IntegerLiteral(_) => {
                let integer_literal = self.parse_integer_literal()?;
                Ok(Expr::new(
                    ExprKind::Literal(integer_literal),
                    *self.token.range(),
                ))
            }
            _ => Err(Diagnostic::new(
                DiagnosticLevel::Error,
                format!("expected expr, found {:?}", self.token.kind()),
                *self.token.range(),
            )),
        }
    }

    pub fn parse_block(&mut self) -> Result<Block> {
        let left_brace = *self.token.range().start();

        if !self.expect(&TokenKind::LeftBrace) {
            return Err(Diagnostic::new(
                DiagnosticLevel::Error,
                format!("expected {{, found {:?}", self.token.kind()),
                left_brace.to(self.token.range().end()),
            ));
        }

        let mut stmts: Vec<Stmt> = Vec::new();

        loop {
            match self.token.kind() {
                TokenKind::EOF => break,
                TokenKind::RightBrace => break,
                _ => {
                    let stmt = self.parse_stmt()?;
                    stmts.push(stmt);
                }
            }
        }

        let right_brace = *self.token.range().end();

        if !self.expect(&TokenKind::RightBrace) {
            return Err(Diagnostic::new(
                DiagnosticLevel::Error,
                format!("expected }}, found {:?}", self.token.kind()),
                right_brace.to(self.token.range().end()),
            ));
        }

        Ok(Block::new(stmts, left_brace.to(&right_brace)))
    }

    pub fn parse_ret_expr(&mut self) -> Result<RetExpr> {
        let ret_keyword = *self.token.range().start();

        if !self.expect(&TokenKind::Keyword(Keyword::Ret)) {
            return Err(Diagnostic::new(
                DiagnosticLevel::Error,
                format!("expected ret keyword, found {:?}", self.token.kind()),
                ret_keyword.to(self.token.range().end()),
            ));
        }

        let expr = self.parse_expr()?;

        Ok(RetExpr::new(expr, ret_keyword.to(self.token.range().end())))
    }

    pub fn parse_integer_literal(&mut self) -> Result<Literal> {
        let x = match self.token.kind() {
            TokenKind::IntegerLiteral(x) => *x,
            _ => {
                return Err(Diagnostic::new(
                    DiagnosticLevel::Error,
                    format!("expected integer literal, found {:?}", self.token.kind()),
                    *self.token.range(),
                ));
            }
        };

        self.advance();

        Ok(Literal::new(LiteralKind::Integer(x), Range::dummy()))
    }
}
