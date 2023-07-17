use crate::ast::expr::Block;
use crate::errors::{Diagnostic, DiagnosticLevel};

pub fn empty_block_error(block: &Block) -> Diagnostic {
    Diagnostic::ranged(
        DiagnosticLevel::Error,
        String::from("block has no statements"),
        block.range,
    )
}
