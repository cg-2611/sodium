use ast::AST;

use crate::ir::IR;
use crate::ty::context::TypeContext;
use crate::type_check::TypeChecker;

pub use self::diagnostics::{SemaError, SemaResult};

pub mod diagnostics;
pub mod ir;
pub mod lower;
pub mod ty;
pub mod type_check;

pub struct Sema;

impl Sema {
    pub fn analysis<'cx>(tcx: TypeContext<'_, 'cx>, ast: AST) -> SemaResult<'cx, IR<'cx>> {
        TypeChecker::type_check(tcx, ast)
    }
}
