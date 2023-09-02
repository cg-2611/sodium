use ast::AST;

use crate::ir::{
    BinaryExpr, BinaryOperator, Block, Decl, DeclKind, Expr, ExprKind, FnDecl, Literal,
    LiteralKind, RetExpr, SourceFile, Stmt, StmtKind, UnaryExpr, UnaryOperator, IR,
};
use crate::lower::ASTLower;
use crate::ty::context::TypeContext;
use crate::ty::Type;
use crate::type_check::inference::TypeInference;
use crate::SemaResult;

pub mod diagnostics;
pub mod inference;

pub struct TypeChecker<'a, 'cx> {
    tcx: TypeContext<'a, 'cx>,
    ast_lower: ASTLower,
}

impl<'a, 'cx> TypeChecker<'a, 'cx> {
    pub fn new(tcx: TypeContext<'a, 'cx>) -> Self {
        Self {
            tcx,
            ast_lower: ASTLower::new(),
        }
    }
}

impl<'cx, 'ast> TypeChecker<'_, 'cx> {
    pub fn type_check(tcx: TypeContext<'_, 'cx>, ast: AST) -> SemaResult<'cx, IR<'cx>> {
        let type_checker = TypeChecker::new(tcx);
        let source_file = type_checker.type_check_source_file(ast.root())?;
        Ok(IR::new(source_file))
    }

    pub fn type_check_source_file(
        &self,
        source_file: &'ast ast::SourceFile,
    ) -> SemaResult<'cx, SourceFile<'cx>> {
        let mut decls = Vec::new();

        for ast_decl in &source_file.decls {
            let decl = self.type_check_decl(ast_decl)?;
            decls.push(decl)
        }

        Ok(self.ast_lower.lower_source_file(source_file, decls))
    }

    pub fn type_check_decl(&self, decl: &'ast ast::Decl) -> SemaResult<'cx, Decl<'cx>> {
        match &decl.kind {
            ast::DeclKind::Fn(ast_fn_decl) => {
                let fn_decl = self.type_check_fn_decl(ast_fn_decl)?;
                let fn_decl_type = fn_decl.ty;
                Ok(self
                    .ast_lower
                    .lower_decl(decl, DeclKind::Fn(fn_decl), fn_decl_type))
            }
        }
    }

    pub fn type_check_fn_decl(&self, fn_decl: &'ast ast::FnDecl) -> SemaResult<'cx, FnDecl<'cx>> {
        let ret_type = TypeInference::ast_type_to_type(self.tcx, &fn_decl.ret_type)?;
        let body = self.type_check_block(&fn_decl.body)?;

        if ret_type != body.ty {
            let message = format!(
                "type mismatch: expected type `{}`, found `{}`",
                ret_type, body.ty
            );

            return Err(self.type_error(message, fn_decl.range));
        }

        let fn_ty = Type::fn_type(self.tcx, ret_type);
        Ok(self.ast_lower.lower_fn_decl(fn_decl, body, fn_ty))
    }

    pub fn type_check_block(&self, block: &'ast ast::Block) -> SemaResult<'cx, Block<'cx>> {
        let mut stmts = Vec::new();

        for ast_stmt in &block.stmts {
            let stmt = self.type_check_stmt(ast_stmt)?;
            stmts.push(stmt);
        }

        if stmts.is_empty() {
            let message = "empty block".to_string();
            return Err(self.type_error(message, block.range));
        }

        let block_ty = stmts.last().unwrap().ty;
        Ok(self.ast_lower.lower_block(block, stmts, block_ty))
    }

    pub fn type_check_stmt(&self, stmt: &'ast ast::Stmt) -> SemaResult<'cx, Stmt<'cx>> {
        match &stmt.kind {
            ast::StmtKind::ExprStmt(ast_expr) => {
                let expr = self.type_check_expr(ast_expr)?;
                let expr_ty = expr.ty;

                Ok(self
                    .ast_lower
                    .lower_stmt(stmt, StmtKind::ExprStmt(expr), expr_ty))
            }
        }
    }

    pub fn type_check_expr(&self, expr: &'ast ast::Expr) -> SemaResult<'cx, Expr<'cx>> {
        match &expr.kind {
            ast::ExprKind::Block(ast_block) => {
                let block = self.type_check_block(ast_block)?;
                let block_ty = block.ty;
                Ok(self
                    .ast_lower
                    .lower_expr(expr, ExprKind::Block(block), block_ty))
            }
            ast::ExprKind::Literal(ast_literal) => {
                let integer_literal = self.type_check_literal(ast_literal)?;
                let literal_ty = integer_literal.ty;
                Ok(self
                    .ast_lower
                    .lower_expr(expr, ExprKind::Literal(integer_literal), literal_ty))
            }
            ast::ExprKind::Unary(ast_unary_expr) => {
                let unary_expr = self.type_check_unary_expr(ast_unary_expr)?;
                let unary_expr_ty = unary_expr.ty;
                Ok(self
                    .ast_lower
                    .lower_expr(expr, ExprKind::Unary(unary_expr), unary_expr_ty))
            }
            ast::ExprKind::Binary(ast_binary_expr) => {
                let binary_expr = self.type_check_binary_expr(ast_binary_expr)?;
                let binary_expr_ty = binary_expr.ty;
                Ok(self
                    .ast_lower
                    .lower_expr(expr, ExprKind::Binary(binary_expr), binary_expr_ty))
            }
            ast::ExprKind::Ret(ast_ret_expr) => {
                let ret_expr = self.type_check_ret_expr(ast_ret_expr)?;
                let ret_expr_ty = ret_expr.ty;
                Ok(self
                    .ast_lower
                    .lower_expr(expr, ExprKind::Ret(ret_expr), ret_expr_ty))
            }
        }
    }

    pub fn type_check_literal(&self, literal: &'ast ast::Literal) -> SemaResult<'cx, Literal<'cx>> {
        match literal.kind {
            ast::LiteralKind::Integer(x) => Ok(self.ast_lower.lower_literal(
                literal,
                LiteralKind::Integer(x),
                Type::i32(self.tcx),
            )),
        }
    }

    pub fn type_check_unary_expr(
        &self,
        unary_expr: &'ast ast::UnaryExpr,
    ) -> SemaResult<'cx, UnaryExpr<'cx>> {
        let operator = match unary_expr.operator {
            ast::UnaryOperator::Negate => UnaryOperator::Negate,
        };

        let expr = self.type_check_expr(&unary_expr.expr)?;
        let expr_ty = expr.ty;

        Ok(self
            .ast_lower
            .lower_unary_expr(unary_expr, operator, Box::new(expr), expr_ty))
    }

    pub fn type_check_binary_expr(
        &self,
        binary_expr: &'ast ast::BinaryExpr,
    ) -> SemaResult<'cx, BinaryExpr<'cx>> {
        let operator = match binary_expr.operator {
            ast::BinaryOperator::Add => BinaryOperator::Add,
            ast::BinaryOperator::Subtract => BinaryOperator::Subtract,
            ast::BinaryOperator::Multiply => BinaryOperator::Multiply,
            ast::BinaryOperator::Divide => BinaryOperator::Divide,
        };

        let lhs = self.type_check_expr(&binary_expr.lhs)?;
        let rhs = self.type_check_expr(&binary_expr.rhs)?;

        if lhs.ty != rhs.ty {
            let message = format!(
                "binary expression type mismatch: lhs `{}`, rhs `{}`",
                lhs.ty, rhs.ty
            );
            return Err(self.type_error(message, binary_expr.range));
        }

        let lhs_ty = lhs.ty;

        Ok(self.ast_lower.lower_binary_expr(
            binary_expr,
            operator,
            Box::new(lhs),
            Box::new(rhs),
            lhs_ty,
        ))
    }

    pub fn type_check_ret_expr(
        &self,
        ret_expr: &'ast ast::RetExpr,
    ) -> SemaResult<'cx, RetExpr<'cx>> {
        let expr = self.type_check_expr(&ret_expr.expr)?;
        let expr_ty = expr.ty;
        Ok(self
            .ast_lower
            .lower_ret_expr(ret_expr, Box::new(expr), expr_ty))
    }
}
