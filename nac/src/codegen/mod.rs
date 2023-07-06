use crate::ast::decl::{Decl, DeclKind, FnDecl};
use crate::ast::expr::{Block, Expr, ExprKind, Literal, LiteralKind, RetExpr};
use crate::ast::stmt::{Stmt, StmtKind};
use crate::ast::ty::Type;
use crate::ast::visitor::Visitor;
use crate::ast::{Identifier, SourceFile, AST};
use crate::llvm::{
    llvm_add_block, llvm_add_function, llvm_build_ret, llvm_const_int, llvm_dispose_builder,
    llvm_dispose_context, llvm_dispose_module, llvm_function_type, llvm_int32_type,
    llvm_position_builder_at_end, llvm_verify_function, Builder, Context, Module, Value,
};

pub struct Codegen {
    context: Context,
    module: Module,
    builder: Builder,
}

impl Codegen {
    pub fn new(module_id: &str) -> Self {
        let context = Context::default();
        let module = Module::new(module_id, &context);
        let builder = Builder::new(&context);

        Self {
            context,
            module,
            builder,
        }
    }

    pub fn codegen(module_id: &str, ast: AST) -> Module {
        let codegen = Codegen::new(module_id);
        codegen.visit_source_file(ast.root());
        codegen.module.write_to_file("main.ll");
        codegen.module.clone()
    }

    pub fn print_llvm_ir(&self) {
        self.module.print();
    }
}

impl Drop for Codegen {
    fn drop(&mut self) {
        llvm_dispose_builder(&self.builder);
        llvm_dispose_module(&self.module);
        llvm_dispose_context(&self.context);
    }
}

impl<'ast> Visitor<'ast, Option<Value>> for Codegen {
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

impl<'ast> Codegen {
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
        let fn_ret_type = llvm_int32_type(&self.context);
        let fn_type = llvm_function_type(fn_ret_type);
        let fn_value = llvm_add_function(&self.module, fn_decl.ident.value.as_str(), fn_type);

        let block = llvm_add_block(&self.context, &fn_value, "entry");
        llvm_position_builder_at_end(&self.builder, &block);

        self.visit_block(&fn_decl.body);

        llvm_verify_function(&fn_value);

        Some(fn_value)
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
            .map(|ret_value| llvm_build_ret(&self.builder, &ret_value))
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
                let int_type = llvm_int32_type(&self.context);
                Some(llvm_const_int(int_type, x, true))
            }
        }
    }
}
