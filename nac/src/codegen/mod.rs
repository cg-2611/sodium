use crate::ast::decl::{Decl, DeclKind, FnDecl};
use crate::ast::expr::{Block, Expr, ExprKind, Literal, LiteralKind, RetExpr};
use crate::ast::stmt::{Stmt, StmtKind};
use crate::ast::ty::Type;
use crate::ast::visitor::Visitor;
use crate::ast::{Identifier, SourceFile, AST};
use crate::llvm::{Builder, Context, Module, Value};

pub struct Codegen<'ctx> {
    context: &'ctx Context,
    module: Module<'ctx>,
    builder: Builder<'ctx>,
}

impl<'ctx> Codegen<'ctx> {
    pub fn new(context: &'ctx Context, module_name: &str) -> Self {
        Self {
            context,
            module: context.create_module(module_name),
            builder: context.create_builder(),
        }
    }

    pub fn codegen<'ast>(
        context: &'ctx Context,
        module_name: &str,
        ast: &'ast AST,
    ) -> Module<'ctx> {
        let codegen = Codegen::new(context, module_name);
        codegen.visit_source_file(ast.root());
        codegen.module
    }

    pub fn print_llvm_ir(&self) {
        self.module.print();
    }
}

impl<'ast, 'ctx> Visitor<'ast, Option<Value>> for Codegen<'ctx> {
    fn visit_source_file(&self, source_file: &'ast SourceFile) -> Option<Value> {
        self.walk_source_file(source_file)
    }

    fn visit_decl(&self, decl: &'ast Decl) -> Option<Value> {
        self.walk_decl(decl)
    }

    fn visit_fn_decl(&self, fn_decl: &'ast FnDecl) -> Option<Value> {
        self.walk_fn_decl(fn_decl)
    }

    fn visit_ident(&self, ident: &'ast Identifier) -> Option<Value> {
        self.walk_ident(ident)
    }

    fn visit_type(&self, ty: &'ast Type) -> Option<Value> {
        self.walk_type(ty)
    }

    fn visit_block(&self, block: &'ast Block) -> Option<Value> {
        self.walk_block(block)
    }

    fn visit_stmt(&self, stmt: &'ast Stmt) -> Option<Value> {
        self.walk_stmt(stmt)
    }

    fn visit_expr(&self, expr: &'ast Expr) -> Option<Value> {
        self.walk_expr(expr)
    }

    fn visit_ret_expr(&self, ret_expr: &'ast RetExpr) -> Option<Value> {
        self.walk_ret_expr(ret_expr)
    }

    fn visit_literal(&self, literal: &'ast Literal) -> Option<Value> {
        self.walk_literal(literal)
    }
}

impl<'ast, 'ctx> Codegen<'ctx> {
    pub fn walk_source_file(&self, source_file: &'ast SourceFile) -> Option<Value> {
        for decl in &source_file.decls {
            self.visit_decl(decl);
        }

        None
    }

    pub fn walk_decl(&self, decl: &'ast Decl) -> Option<Value> {
        match &decl.kind {
            DeclKind::Fn(fn_decl) => self.visit_fn_decl(fn_decl),
        }
    }

    pub fn walk_fn_decl(&self, fn_decl: &'ast FnDecl) -> Option<Value> {
        let fn_ret_type = self.context.i32_type();
        let fn_type = fn_ret_type.fn_type();
        let fn_value = self
            .module
            .add_function(fn_decl.ident.value.as_str(), &fn_type);

        let block = self.context.append_basic_block(&fn_value, "entry");
        self.builder.position_at_end(&block);

        self.visit_block(&fn_decl.body);

        match fn_value.verify_fn() {
            Ok(_) => Some(fn_value),
            Err(_) => None,
        }
    }

    pub fn walk_ident(&self, ident: &'ast Identifier) -> Option<Value> {
        let _ = ident;
        None
    }

    pub fn walk_type(&self, ty: &'ast Type) -> Option<Value> {
        let _ = ty;
        None
    }

    pub fn walk_block(&self, block: &'ast Block) -> Option<Value> {
        for stmt in block.stmts.split_last().unwrap().1 {
            self.visit_stmt(stmt);
        }

        let ret_expr_stmt = block.stmts.last().unwrap();
        self.visit_stmt(ret_expr_stmt)
            .map(|ret_value| self.builder.build_ret(&ret_value))
    }

    pub fn walk_stmt(&self, stmt: &'ast Stmt) -> Option<Value> {
        match &stmt.kind {
            StmtKind::ExprStmt(expr) => self.visit_expr(expr),
        }
    }

    pub fn walk_expr(&self, expr: &'ast Expr) -> Option<Value> {
        match &expr.kind {
            ExprKind::Block(_) => None,
            ExprKind::Ret(ret_expr) => self.visit_ret_expr(ret_expr),
            ExprKind::Literal(literal) => self.visit_literal(literal),
        }
    }

    pub fn walk_ret_expr(&self, ret_expr: &'ast RetExpr) -> Option<Value> {
        self.visit_expr(&ret_expr.expr)
    }

    pub fn walk_literal(&self, literal: &'ast Literal) -> Option<Value> {
        match literal.kind {
            LiteralKind::Integer(x) => {
                let int_type = self.context.i32_type();
                Some(int_type.const_int(x as u64))
            }
        }
    }
}
