use crate::codegen::CodeGen;
use crate::errors::NACResult;
use crate::lexer::Lexer;
use crate::llvm::Context;
use crate::parser::Parser;
use crate::session::Session;
use crate::source::file::SourceFileReader;
use crate::target::TargetGen;

const EXIT_SUCCESS: i32 = 0;
const EXIT_FAILURE: i32 = 1;

fn catch_unwind_with_exit_code(f: impl FnOnce() -> NACResult<()>) -> i32 {
    let result = f();
    match result {
        Ok(_) => EXIT_SUCCESS,
        Err(_) => EXIT_FAILURE,
    }
}

fn compile_file(session: &Session, path: &str) -> NACResult<()> {
    let src_file = session.run_pass(|| SourceFileReader::source_file_from_path(session, path))?;

    let token_stream = session.run_pass(|| Lexer::tokenize(session, src_file.contents()))?;

    let ast = session.run_pass(|| Parser::parse(session, token_stream))?;

    let context = Context::create();
    let module = session.run_pass(|| CodeGen::codegen(session, &context, "module", &ast))?;

    session.run_pass(|| TargetGen::compile_module(session, &module))?;

    Ok(())
}

pub fn main() {
    let exit_code = catch_unwind_with_exit_code(|| {
        let session = Session::new();

        let args: Vec<String> = std::env::args().collect();
        if args.len() < 2 {
            return Err(session
                .create_error("invalid arguments passed".to_string())
                .emit());
        }

        compile_file(&session, &args[1])
    });

    std::process::exit(exit_code);
}
