use crate::ast::expr::{Block, Expr, ExprKind, Literal, LiteralKind, RetExpr};
use crate::ast::stmt::Stmt;
use crate::errors::Result;
use crate::parser::Parser;
use crate::token::{Keyword, TokenKind};

impl<'s> Parser<'s> {
    pub fn parse_expr(&mut self) -> Result<Box<Expr>> {
        match self.token.kind {
            TokenKind::LeftBrace => {
                let block = self.parse_block()?;
                let block_range = block.range;
                Ok(Box::new(Expr::new(ExprKind::Block(block), block_range)))
            }
            TokenKind::Keyword(Keyword::Ret) => {
                let ret_expr = self.parse_ret_expr()?;
                let ret_expr_range = ret_expr.range;
                Ok(Box::new(Expr::new(ExprKind::Ret(ret_expr), ret_expr_range)))
            }
            TokenKind::IntegerLiteral(_) => {
                let integer_literal = self.parse_integer_literal()?;
                let integer_literal_range = integer_literal.range;
                Ok(Box::new(Expr::new(
                    ExprKind::Literal(integer_literal),
                    integer_literal_range,
                )))
            }
            _ => Err(self.error("expected expr", self.token.range)),
        }
    }

    pub fn parse_block(&mut self) -> Result<Box<Block>> {
        let left_brace = self.expect(TokenKind::LeftBrace)?;
        let mut stmts: Vec<Stmt> = Vec::new();

        while self.token.kind != TokenKind::RightBrace {
            let stmt = self.parse_stmt()?;
            stmts.push(stmt);
        }

        let right_brace = self.expect(TokenKind::RightBrace)?;
        Ok(Box::new(Block::new(stmts, left_brace.to(right_brace))))
    }

    pub fn parse_ret_expr(&mut self) -> Result<Box<RetExpr>> {
        let ret_keyword = self.expect(TokenKind::Keyword(Keyword::Ret))?;
        let expr = self.parse_expr()?;
        let expr_range = expr.range;
        Ok(Box::new(RetExpr::new(expr, ret_keyword.to(expr_range))))
    }

    pub fn parse_integer_literal(&mut self) -> Result<Literal> {
        if let TokenKind::IntegerLiteral(x) = self.token.kind {
            let integer_literal = Ok(Literal {
                kind: LiteralKind::Integer(x),
                range: self.token.range,
            });

            self.advance();
            integer_literal
        } else {
            self.expected_integer_literal()
        }
    }
}
