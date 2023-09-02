use ast::expr::{BinaryExpr, BinaryOperator, UnaryExpr, UnaryOperator};
use ast::{Block, Expr, ExprKind, Literal, LiteralKind, RetExpr, Stmt};
use token::{Keyword, TokenKind};

use crate::{Parser, ParserResult};

impl<'a> Parser<'a> {
    pub fn parse_expr(&mut self) -> ParserResult<'a, Expr> {
        self.parse_binary_expr()
    }

    pub fn parse_binary_expr(&mut self) -> ParserResult<'a, Expr> {
        let lhs = self.parse_unary_expr()?;

        match self.token.kind {
            TokenKind::Add => self.parse_add_expr(lhs),
            TokenKind::Subtract => self.parse_subtract_expr(lhs),
            TokenKind::Multiply => self.parse_multiply_expr(lhs),
            TokenKind::Divide => self.parse_divide_expr(lhs),
            _ => Ok(lhs),
        }
    }

    pub fn parse_unary_expr(&mut self) -> ParserResult<'a, Expr> {
        match self.token.kind {
            TokenKind::Subtract => self.parse_negate_expr(),
            _ => self.parse_primary_expr(),
        }
    }

    pub fn parse_negate_expr(&mut self) -> ParserResult<'a, Expr> {
        let negate = self.expect(TokenKind::Subtract)?;
        let expr = self.parse_expr()?;

        let negate_expr_range = negate.to(expr.range);
        let negate_expr = UnaryExpr::new(UnaryOperator::Negate, Box::new(expr), negate_expr_range);

        Ok(Expr::new(ExprKind::Unary(negate_expr), negate_expr_range))
    }

    pub fn parse_primary_expr(&mut self) -> ParserResult<'a, Expr> {
        match self.token.kind {
            TokenKind::LeftBrace => {
                let block = self.parse_block()?;
                let block_range = block.range;
                Ok(Expr::new(ExprKind::Block(block), block_range))
            }
            TokenKind::IntegerLiteral(_) => {
                let integer_literal = self.parse_integer_literal()?;
                let integer_literal_range = integer_literal.range;
                Ok(Expr::new(
                    ExprKind::Literal(integer_literal),
                    integer_literal_range,
                ))
            }
            TokenKind::Keyword(Keyword::Ret) => {
                let ret_expr = self.parse_ret_expr()?;
                let ret_expr_range = ret_expr.range;
                Ok(Expr::new(ExprKind::Ret(ret_expr), ret_expr_range))
            }
            _ => Err(self.parser_error("expected expr", self.token.range)),
        }
    }

    pub fn parse_add_expr(&mut self, lhs: Expr) -> ParserResult<'a, Expr> {
        self.expect(TokenKind::Add)?;
        let rhs = self.parse_expr()?;
        let add_expr_range = lhs.range.to(rhs.range);
        let add_expr = BinaryExpr::new(
            BinaryOperator::Add,
            Box::new(lhs),
            Box::new(rhs),
            add_expr_range,
        );

        Ok(Expr::new(ExprKind::Binary(add_expr), add_expr_range))
    }

    pub fn parse_subtract_expr(&mut self, lhs: Expr) -> ParserResult<'a, Expr> {
        self.expect(TokenKind::Subtract)?;
        let rhs = self.parse_expr()?;
        let subtract_expr_range = lhs.range.to(rhs.range);
        let subtract_expr = BinaryExpr::new(
            BinaryOperator::Subtract,
            Box::new(lhs),
            Box::new(rhs),
            subtract_expr_range,
        );

        Ok(Expr::new(
            ExprKind::Binary(subtract_expr),
            subtract_expr_range,
        ))
    }

    pub fn parse_multiply_expr(&mut self, lhs: Expr) -> ParserResult<'a, Expr> {
        self.expect(TokenKind::Multiply)?;
        let rhs = self.parse_expr()?;
        let multiply_expr_range = lhs.range.to(rhs.range);
        let multiply_expr = BinaryExpr::new(
            BinaryOperator::Multiply,
            Box::new(lhs),
            Box::new(rhs),
            multiply_expr_range,
        );

        Ok(Expr::new(
            ExprKind::Binary(multiply_expr),
            multiply_expr_range,
        ))
    }

    pub fn parse_divide_expr(&mut self, lhs: Expr) -> ParserResult<'a, Expr> {
        self.expect(TokenKind::Divide)?;
        let rhs = self.parse_expr()?;
        let divide_expr_range = lhs.range.to(rhs.range);
        let divide_expr = BinaryExpr::new(
            BinaryOperator::Divide,
            Box::new(lhs),
            Box::new(rhs),
            divide_expr_range,
        );

        Ok(Expr::new(ExprKind::Binary(divide_expr), divide_expr_range))
    }

    pub fn parse_block(&mut self) -> ParserResult<'a, Block> {
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
        Ok(Block::new(stmts, left_brace.to(right_brace)))
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

    pub fn parse_ret_expr(&mut self) -> ParserResult<'a, RetExpr> {
        let ret_keyword = self.expect(TokenKind::Keyword(Keyword::Ret))?;
        let expr = self.parse_expr()?;
        let expr_range = expr.range;
        Ok(RetExpr::new(Box::new(expr), ret_keyword.to(expr_range)))
    }
}
