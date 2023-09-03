use codegen::CodeGen;
use errors::{Diagnostic, ErrorOccurred, NACResult};
use lexer::Lexer;
use llvm::LLVMContext;
use parser::Parser;
use sema::ty::{TypeArena, TypeContext};
use sema::Sema;
use session::Session;
use source::SourceFileReader;
use target::TargetGen;

pub fn run_pass<'a, T>(
    sess: &Session,
    pass: impl FnOnce() -> Result<T, Diagnostic<'a, ErrorOccurred>>,
) -> NACResult<T> {
    let result = pass().map_err(|mut error| error.emit())?;
    sess.has_errors()?;
    Ok(result)
}

pub fn compile_file(path: &str) -> NACResult<()> {
    let sess = Session::new(path)?;

    let src_file = run_pass(&sess, || {
        SourceFileReader::source_file_from_path(&sess, sess.input().path())
    })?;

    let token_stream = run_pass(&sess, || Lexer::tokenize(&sess, src_file.contents()))?;
    let ast = run_pass(&sess, || Parser::parse(&sess, token_stream))?;

    let type_arena = TypeArena::new();

    let ir = TypeContext::create_and_enter(&sess, &type_arena, |tcx| {
        run_pass(&sess, || Sema::analysis(tcx, ast))
    })?;

    let llvm_context = LLVMContext::create();
    let module = run_pass(&sess, || {
        CodeGen::codegen(&sess, &llvm_context, sess.output().name(), ir)
    })?;

    run_pass(&sess, || TargetGen::compile_module(&sess, &module))?;

    Ok(())
}
