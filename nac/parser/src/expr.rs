use ast::{Block, Expr, ExprKind, Literal, LiteralKind, RetExpr, Stmt};
use token::{Keyword, TokenKind};

use crate::{Parser, ParserResult};

impl<'a> Parser<'a> {
    pub fn parse_expr(&mut self) -> ParserResult<'a, Box<Expr>> {
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
            _ => Err(self.parser_error("expected expr", self.token.range)),
        }
    }

    pub fn parse_block(&mut self) -> ParserResult<'a, Box<Block>> {
        let left_brace = self.expect(TokenKind::LeftBrace)?;
        let mut stmts: Vec<Stmt> = Vec::new();

        while self.token.kind != TokenKind::RightBrace && self.token.kind != TokenKind::EOF {
            let stmt = self.parse_stmt();

            match stmt {
                Ok(stmt) => stmts.push(stmt),
                Err(mut diagnostic) => {
                    diagnostic.emit();
                    self.recover_stmt();
                }
            }
        }

        let right_brace = self.expect(TokenKind::RightBrace)?;
        Ok(Box::new(Block::new(stmts, left_brace.to(right_brace))))
    }

    pub fn parse_ret_expr(&mut self) -> ParserResult<'a, Box<RetExpr>> {
        let ret_keyword = self.expect(TokenKind::Keyword(Keyword::Ret))?;
        let expr = self.parse_expr()?;
        let expr_range = expr.range;
        Ok(Box::new(RetExpr::new(expr, ret_keyword.to(expr_range))))
    }

    pub fn parse_integer_literal(&mut self) -> ParserResult<'a, Literal> {
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
