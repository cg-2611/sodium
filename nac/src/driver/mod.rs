use std::process;

use crate::ast::printer::ASTPrinter;
use crate::errors::{Diagnostic, DiagnosticLevel, Result};
use crate::lexer::Lexer;
use crate::parser::Parser;
use crate::session::Session;
use crate::source::SourceFile;

const EXIT_SUCCESS: i32 = 0;
const EXIT_FAILURE: i32 = 1;

fn catch_unwind(f: impl FnOnce() -> Result<()>) -> i32 {
    let result = f();
    match result {
        Ok(_) => EXIT_SUCCESS,
        Err(_) => EXIT_FAILURE,
    }
}

fn compile_source_file(session: &Session, src: &SourceFile) -> Result<()> {
    let token_stream = Lexer::tokenize(session, src.contents());

    if session.has_errors() {
        session.emit_diagnostics();
        return Err(Diagnostic::without_range(
            DiagnosticLevel::Fatal,
            String::from("compilation error"),
        ));
    }

    let ast = Parser::parse(session, token_stream);

    if session.has_errors() {
        session.emit_diagnostics();
        return Err(Diagnostic::without_range(
            DiagnosticLevel::Fatal,
            String::from("compilation error"),
        ));
    }

    let ast = ast.unwrap();
    ASTPrinter::print_ast(&ast);

    Ok(())
}

pub fn main() {
    let exit_code = catch_unwind(|| {
        let args: Vec<String> = std::env::args().collect();
        if args.len() < 2 {
            return Err(Diagnostic::without_range(
                DiagnosticLevel::Fatal,
                String::from("invalid arguments passed"),
            ));
        }

        let session = Session::new();
        let src = match SourceFile::from(&args[1]) {
            Ok(src) => src,
            Err(diagnostic) => {
                session.report_diagnostic(diagnostic);
                session.emit_diagnostics();
                return Err(Diagnostic::without_range(
                    DiagnosticLevel::Fatal,
                    String::from("compilation error"),
                ));
            }
        };

        compile_source_file(&session, &src)
    });

    process::exit(exit_code);
}
