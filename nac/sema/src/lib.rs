use ast::AST;
use context::CompilerContext;
use ir::IR;

use crate::lower::ASTLower;
use crate::type_check::TypeChecker;

pub use self::diagnostics::{SemaError, SemaResult};

pub mod diagnostics;
pub mod inference;
pub mod lower;
pub mod type_check;

pub struct Sema<'ctx> {
    context: &'ctx CompilerContext<'ctx>,
}

impl<'ctx> Sema<'ctx> {
    pub fn new(context: &'ctx CompilerContext) -> Self {
        Self { context }
    }

    pub fn semantic_analysis(context: &'ctx CompilerContext, ast: AST) -> SemaResult<'ctx, IR> {
        let ir = Sema::lower_ast(context, ast)?;
        let ir = Sema::type_check_ir(context, ir)?;

        Ok(ir)
    }

    pub fn lower_ast(context: &'ctx CompilerContext, ast: AST) -> SemaResult<'ctx, IR> {
        ASTLower::lower(context, &ast)
    }

    pub fn type_check_ir(context: &'ctx CompilerContext, ir: IR) -> SemaResult<'ctx, IR> {
        TypeChecker::type_check(context, ir)
    }
}
