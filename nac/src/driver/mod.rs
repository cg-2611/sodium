use crate::codegen::CodeGen;
use crate::errors::{Diagnostic, DiagnosticLevel, Result};
use crate::lexer::Lexer;
use crate::llvm::Context;
use crate::parser::Parser;
use crate::session::Session;
use crate::source::SourceFile;
use crate::target::TargetGen;

const EXIT_SUCCESS: i32 = 0;
const EXIT_FAILURE: i32 = 1;

fn catch_with_exit_code(f: impl FnOnce() -> Result<()>) -> i32 {
    let result = f();
    match result {
        Ok(_) => EXIT_SUCCESS,
        Err(_) => EXIT_FAILURE,
    }
}

fn early_exit(session: &Session) -> Result<()> {
    if session.has_errors() {
        session.emit_diagnostics();
        return Err(Diagnostic::new(
            DiagnosticLevel::Fatal,
            String::from("early exit"),
        ));
    }

    Ok(())
}

fn compile_source_file(session: &Session, src: &SourceFile) -> Result<()> {
    let token_stream = Lexer::tokenize(session, src.contents())?;
    early_exit(session)?;

    let ast = Parser::parse(session, token_stream)?;
    early_exit(session)?;

    let context = Context::create();
    let module = CodeGen::codegen(session, &context, "module", &ast)?;
    early_exit(session)?;

    TargetGen::compile_module(session, &module)?;
    early_exit(session)?;

    Ok(())
}

pub fn main() {
    let exit_code = catch_with_exit_code(|| {
        let session = Session::new();

        let args: Vec<String> = std::env::args().collect();
        if args.len() < 2 {
            let error = Diagnostic::new(
                DiagnosticLevel::Fatal,
                String::from("invalid arguments passed"),
            );

            session.report_diagnostic(error);
            return early_exit(&session);
        }

        let src = match SourceFile::from(&args[1]) {
            Ok(src) => src,
            Err(diagnostic) => {
                session.report_diagnostic(diagnostic);
                return early_exit(&session);
            }
        };

        compile_source_file(&session, &src)
    });

    std::process::exit(exit_code);
}
