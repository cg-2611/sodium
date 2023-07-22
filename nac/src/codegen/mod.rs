use crate::ast::decl::{Decl, DeclKind, FnDecl};
use crate::ast::expr::{Block, Expr, ExprKind, Literal, LiteralKind, RetExpr};
use crate::ast::stmt::{Stmt, StmtKind};
use crate::ast::ty::Type;
use crate::ast::visitor::Visitor;
use crate::ast::{Identifier, SourceFile, AST};
use crate::llvm::{Builder, Context, Module, Value};
use crate::session::Session;

pub use self::diagnostics::{CodeGenError, CodeGenResult};

#[cfg(test)]
mod tests;

pub mod diagnostics;

pub struct CodeGen<'a, 'ctx> {
    session: &'a Session,
    context: &'ctx Context,
    module: Module<'ctx>,
    builder: Builder<'ctx>,
}

impl<'a, 'ctx> CodeGen<'a, 'ctx> {
    pub fn new(session: &'a Session, context: &'ctx Context, module_name: &str) -> Self {
        Self {
            session,
            context,
            module: context.create_module(module_name),
            builder: context.create_builder(),
        }
    }

    pub fn codegen<'ast>(
        session: &'a Session,
        context: &'ctx Context,
        module_name: &str,
        ast: &'ast AST,
    ) -> CodeGenResult<'a, Module<'ctx>> {
        let codegen = Self::new(session, context, module_name);
        codegen.visit_source_file(ast.root())?;
        Ok(codegen.module)
    }

    pub fn print_llvm_ir(&self) {
        self.module.print();
    }
}

impl<'a, 'ctx, 'ast> Visitor<'ast, CodeGenResult<'a, Option<Value>>> for CodeGen<'a, 'ctx> {
    fn visit_source_file(&self, source_file: &'ast SourceFile) -> CodeGenResult<'a, Option<Value>> {
        self.walk_source_file(source_file)
    }

    fn visit_decl(&self, decl: &'ast Decl) -> CodeGenResult<'a, Option<Value>> {
        self.walk_decl(decl)
    }

    fn visit_fn_decl(&self, fn_decl: &'ast FnDecl) -> CodeGenResult<'a, Option<Value>> {
        self.walk_fn_decl(fn_decl)
    }

    fn visit_ident(&self, ident: &'ast Identifier) -> CodeGenResult<'a, Option<Value>> {
        self.walk_ident(ident)
    }

    fn visit_type(&self, ty: &'ast Type) -> CodeGenResult<'a, Option<Value>> {
        self.walk_type(ty)
    }

    fn visit_block(&self, block: &'ast Block) -> CodeGenResult<'a, Option<Value>> {
        self.walk_block(block)
    }

    fn visit_stmt(&self, stmt: &'ast Stmt) -> CodeGenResult<'a, Option<Value>> {
        self.walk_stmt(stmt)
    }

    fn visit_expr(&self, expr: &'ast Expr) -> CodeGenResult<'a, Option<Value>> {
        self.walk_expr(expr)
    }

    fn visit_ret_expr(&self, ret_expr: &'ast RetExpr) -> CodeGenResult<'a, Option<Value>> {
        self.walk_ret_expr(ret_expr)
    }

    fn visit_literal(&self, literal: &'ast Literal) -> CodeGenResult<'a, Option<Value>> {
        self.walk_literal(literal)
    }
}

impl<'a, 'ctx, 'ast> CodeGen<'a, 'ctx> {
    pub fn walk_source_file(
        &self,
        source_file: &'ast SourceFile,
    ) -> CodeGenResult<'a, Option<Value>> {
        for decl in &source_file.decls {
            let result = self.visit_decl(decl);
            if let Some(mut diagnostic) = result.err() {
                diagnostic.emit();
            }
        }

        Ok(None)
    }

    pub fn walk_decl(&self, decl: &'ast Decl) -> CodeGenResult<'a, Option<Value>> {
        match &decl.kind {
            DeclKind::Fn(fn_decl) => self.visit_fn_decl(fn_decl),
        }
    }

    pub fn walk_fn_decl(&self, fn_decl: &'ast FnDecl) -> CodeGenResult<'a, Option<Value>> {
        let fn_ret_type = self.context.i32_type();
        let fn_type = fn_ret_type.fn_type();
        let fn_value = self
            .module
            .add_function(fn_decl.ident.value.as_str(), &fn_type);

        let block = self.context.append_basic_block(&fn_value, "entry");
        self.builder.position_at_end(&block);

        self.visit_block(&fn_decl.body)?;

        fn_value
            .verify_fn()
            .map_err(|message| self.codegen_error(&message.as_string(), fn_decl.range))?;

        Ok(Some(fn_value))
    }

    pub fn walk_ident(&self, ident: &'ast Identifier) -> CodeGenResult<'a, Option<Value>> {
        let _ = ident;
        Ok(None)
    }

    pub fn walk_type(&self, ty: &'ast Type) -> CodeGenResult<'a, Option<Value>> {
        let _ = ty;
        Ok(None)
    }

    pub fn walk_block(&self, block: &'ast Block) -> CodeGenResult<'a, Option<Value>> {
        if block.stmts.is_empty() {
            return Err(self.codegen_error("empty block", block.range));
        }

        let (last_stmt, stmts) = block.stmts.split_last().unwrap();

        for stmt in stmts {
            self.visit_stmt(stmt)?;
        }

        self.visit_stmt(last_stmt)
            .map(|ret_value| Some(self.builder.build_ret(ret_value)))
    }

    pub fn walk_stmt(&self, stmt: &'ast Stmt) -> CodeGenResult<'a, Option<Value>> {
        match &stmt.kind {
            StmtKind::ExprStmt(expr) => self.visit_expr(expr),
        }
    }

    pub fn walk_expr(&self, expr: &'ast Expr) -> CodeGenResult<'a, Option<Value>> {
        match &expr.kind {
            ExprKind::Block(_) => Ok(None),
            ExprKind::Ret(ret_expr) => self.visit_ret_expr(ret_expr),
            ExprKind::Literal(literal) => self.visit_literal(literal),
        }
    }

    pub fn walk_ret_expr(&self, ret_expr: &'ast RetExpr) -> CodeGenResult<'a, Option<Value>> {
        self.visit_expr(&ret_expr.expr)
    }

    pub fn walk_literal(&self, literal: &'ast Literal) -> CodeGenResult<'a, Option<Value>> {
        let literal = match literal.kind {
            LiteralKind::Integer(x) => {
                let int_type = self.context.i32_type();
                Some(int_type.const_int(x as u64))
            }
        };

        Ok(literal)
    }
}
