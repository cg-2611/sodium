use llvm::{LLVMBuilder, LLVMContext, LLVMModule, Type, Value};
use sema::ir::{
    Block, Decl, DeclKind, Expr, ExprKind, FnDecl, Identifier, Literal, LiteralKind, RetExpr,
    SourceFile, Stmt, StmtKind, IR,
};
use sema::ty;
use session::Session;

pub use self::diagnostics::{CodeGenError, CodeGenResult};

pub mod diagnostics;

pub struct CodeGen<'cx> {
    sess: &'cx Session,
    llvm_context: &'cx LLVMContext,
    module: LLVMModule<'cx>,
    builder: LLVMBuilder<'cx>,
}

impl<'cx, 'ir> CodeGen<'cx> {
    pub fn new(sess: &'cx Session, llvm_context: &'cx LLVMContext, module_name: &str) -> Self {
        Self {
            sess,
            llvm_context,
            module: llvm_context.create_module(module_name),
            builder: llvm_context.create_builder(),
        }
    }

    pub fn codegen(
        session: &'cx Session,
        llvm_context: &'cx LLVMContext,
        module_name: &str,
        ir: IR<'_>,
    ) -> CodeGenResult<'cx, LLVMModule<'cx>> {
        let codegen = Self::new(session, llvm_context, module_name);
        codegen.codegen_source_file(ir.source_file())?;
        Ok(codegen.module)
    }

    pub fn print_llvm_ir(&self) {
        self.module.print();
    }

    pub fn codegen_source_file(
        &self,
        source_file: &'ir SourceFile<'_>,
    ) -> CodeGenResult<'cx, Option<Value>> {
        for decl in &source_file.decls {
            let result = self.codegen_decl(decl);
            if let Some(mut diagnostic) = result.err() {
                diagnostic.emit();
            }
        }

        Ok(None)
    }

    pub fn codegen_decl(&self, decl: &'ir Decl<'_>) -> CodeGenResult<'cx, Option<Value>> {
        match &decl.kind {
            DeclKind::Fn(fn_decl) => self.codegen_fn_decl(fn_decl),
        }
    }

    pub fn codegen_fn_decl(&self, fn_decl: &'ir FnDecl<'_>) -> CodeGenResult<'cx, Option<Value>> {
        let fn_type = self.type_to_llvm_type(&fn_decl.ty)?.fn_type();
        let fn_value = self
            .module
            .add_function(fn_decl.ident.value.as_str(), &fn_type);

        let block = self.llvm_context.append_basic_block(&fn_value, "entry");
        self.builder.position_at_end(&block);

        self.codegen_block(&fn_decl.body)?;

        fn_value
            .verify_fn()
            .map_err(|message| self.codegen_error(&message.as_string(), fn_decl.range))?;

        Ok(Some(fn_value))
    }

    pub fn codegen_ident(&self, ident: &'ir Identifier) -> CodeGenResult<'cx, Option<Value>> {
        let _ = ident;
        Ok(None)
    }

    pub fn codegen_block(&self, block: &'ir Block<'_>) -> CodeGenResult<'cx, Option<Value>> {
        let (last_stmt, stmts) = block.stmts.split_last().unwrap();

        for stmt in stmts {
            self.codegen_stmt(stmt)?;
        }

        self.codegen_stmt(last_stmt)
            .map(|ret_value| Some(self.builder.build_ret(ret_value)))
    }

    pub fn codegen_stmt(&self, stmt: &'ir Stmt<'_>) -> CodeGenResult<'cx, Option<Value>> {
        match &stmt.kind {
            StmtKind::ExprStmt(expr) => self.codegen_expr(expr),
        }
    }

    pub fn codegen_expr(&self, expr: &'ir Expr<'_>) -> CodeGenResult<'cx, Option<Value>> {
        match &expr.kind {
            ExprKind::Block(block) => self.codegen_block(block),
            ExprKind::Ret(ret_expr) => self.codegen_ret_expr(ret_expr),
            ExprKind::Literal(literal) => self.codegen_literal(literal),
        }
    }

    pub fn codegen_ret_expr(
        &self,
        ret_expr: &'ir RetExpr<'_>,
    ) -> CodeGenResult<'cx, Option<Value>> {
        self.codegen_expr(&ret_expr.expr)
    }

    pub fn codegen_literal(&self, literal: &'ir Literal<'_>) -> CodeGenResult<'cx, Option<Value>> {
        let literal = match literal.kind {
            LiteralKind::Integer(x) => {
                let int_type = self.type_to_llvm_type(&literal.ty)?;
                Some(int_type.const_int(x as u64))
            }
        };

        Ok(literal)
    }

    pub fn type_to_llvm_type(&self, ty: &ty::Type<'_>) -> CodeGenResult<'cx, Type> {
        match ty.kind() {
            ty::TypeKind::I32 => Ok(self.llvm_context.i32_type()),
            ty::TypeKind::Fn(ret_type) => self.type_to_llvm_type(ret_type),
        }
    }
}
