use ast::decl::{Decl, DeclKind, FnDecl};
use ast::expr::{Block, Expr, ExprKind, Literal, LiteralKind, RetExpr};
use ast::stmt::{Stmt, StmtKind};
use ast::ty::Type;
use ast::visitor::Visitor;
use ast::{Identifier, SourceFile, AST};
use context::CompilerContext;
use llvm::{Builder, Module, Value};

pub use self::diagnostics::{CodeGenError, CodeGenResult};

pub mod diagnostics;

pub struct CodeGen<'ctx> {
    context: &'ctx CompilerContext<'ctx>,
    module: Module<'ctx>,
    builder: Builder<'ctx>,
}

impl<'ctx> CodeGen<'ctx> {
    pub fn new(context: &'ctx CompilerContext, module_name: &str) -> Self {
        Self {
            context,
            module: context.llvm_context().create_module(module_name),
            builder: context.llvm_context().create_builder(),
        }
    }

    pub fn codegen<'ast>(
        context: &'ctx CompilerContext,
        module_name: &str,
        ast: &'ast AST,
    ) -> CodeGenResult<'ctx, Module<'ctx>> {
        let codegen = Self::new(context, module_name);
        codegen.visit_source_file(ast.root())?;
        Ok(codegen.module)
    }

    pub fn print_llvm_ir(&self) {
        self.module.print();
    }
}

impl<'ctx, 'ast> Visitor<'ast, CodeGenResult<'ctx, Option<Value>>> for CodeGen<'ctx> {
    fn visit_source_file(
        &self,
        source_file: &'ast SourceFile,
    ) -> CodeGenResult<'ctx, Option<Value>> {
        self.walk_source_file(source_file)
    }

    fn visit_decl(&self, decl: &'ast Decl) -> CodeGenResult<'ctx, Option<Value>> {
        self.walk_decl(decl)
    }

    fn visit_fn_decl(&self, fn_decl: &'ast FnDecl) -> CodeGenResult<'ctx, Option<Value>> {
        self.walk_fn_decl(fn_decl)
    }

    fn visit_ident(&self, ident: &'ast Identifier) -> CodeGenResult<'ctx, Option<Value>> {
        self.walk_ident(ident)
    }

    fn visit_type(&self, ty: &'ast Type) -> CodeGenResult<'ctx, Option<Value>> {
        self.walk_type(ty)
    }

    fn visit_block(&self, block: &'ast Block) -> CodeGenResult<'ctx, Option<Value>> {
        self.walk_block(block)
    }

    fn visit_stmt(&self, stmt: &'ast Stmt) -> CodeGenResult<'ctx, Option<Value>> {
        self.walk_stmt(stmt)
    }

    fn visit_expr(&self, expr: &'ast Expr) -> CodeGenResult<'ctx, Option<Value>> {
        self.walk_expr(expr)
    }

    fn visit_ret_expr(&self, ret_expr: &'ast RetExpr) -> CodeGenResult<'ctx, Option<Value>> {
        self.walk_ret_expr(ret_expr)
    }

    fn visit_literal(&self, literal: &'ast Literal) -> CodeGenResult<'ctx, Option<Value>> {
        self.walk_literal(literal)
    }
}

impl<'ctx, 'ast> CodeGen<'ctx> {
    pub fn walk_source_file(
        &self,
        source_file: &'ast SourceFile,
    ) -> CodeGenResult<'ctx, Option<Value>> {
        for decl in &source_file.decls {
            let result = self.visit_decl(decl);
            if let Some(mut diagnostic) = result.err() {
                diagnostic.emit();
            }
        }

        Ok(None)
    }

    pub fn walk_decl(&self, decl: &'ast Decl) -> CodeGenResult<'ctx, Option<Value>> {
        match &decl.kind {
            DeclKind::Fn(fn_decl) => self.visit_fn_decl(fn_decl),
        }
    }

    pub fn walk_fn_decl(&self, fn_decl: &'ast FnDecl) -> CodeGenResult<'ctx, Option<Value>> {
        let fn_ret_type = self.context.llvm_context().i32_type();
        let fn_type = fn_ret_type.fn_type();
        let fn_value = self
            .module
            .add_function(fn_decl.ident.value.as_str(), &fn_type);

        let block = self
            .context
            .llvm_context()
            .append_basic_block(&fn_value, "entry");
        self.builder.position_at_end(&block);

        self.visit_block(&fn_decl.body)?;

        fn_value
            .verify_fn()
            .map_err(|message| self.codegen_error(&message.as_string(), fn_decl.range))?;

        Ok(Some(fn_value))
    }

    pub fn walk_ident(&self, ident: &'ast Identifier) -> CodeGenResult<'ctx, Option<Value>> {
        let _ = ident;
        Ok(None)
    }

    pub fn walk_type(&self, ty: &'ast Type) -> CodeGenResult<'ctx, Option<Value>> {
        let _ = ty;
        Ok(None)
    }

    pub fn walk_block(&self, block: &'ast Block) -> CodeGenResult<'ctx, Option<Value>> {
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

    pub fn walk_stmt(&self, stmt: &'ast Stmt) -> CodeGenResult<'ctx, Option<Value>> {
        match &stmt.kind {
            StmtKind::ExprStmt(expr) => self.visit_expr(expr),
        }
    }

    pub fn walk_expr(&self, expr: &'ast Expr) -> CodeGenResult<'ctx, Option<Value>> {
        match &expr.kind {
            ExprKind::Block(_) => Ok(None),
            ExprKind::Ret(ret_expr) => self.visit_ret_expr(ret_expr),
            ExprKind::Literal(literal) => self.visit_literal(literal),
        }
    }

    pub fn walk_ret_expr(&self, ret_expr: &'ast RetExpr) -> CodeGenResult<'ctx, Option<Value>> {
        self.visit_expr(&ret_expr.expr)
    }

    pub fn walk_literal(&self, literal: &'ast Literal) -> CodeGenResult<'ctx, Option<Value>> {
        let literal = match literal.kind {
            LiteralKind::Integer(x) => {
                let int_type = self.context.llvm_context().i32_type();
                Some(int_type.const_int(x as u64))
            }
        };

        Ok(literal)
    }
}
