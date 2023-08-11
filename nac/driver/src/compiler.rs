use codegen::CodeGen;
use context::CompilerContext;
use errors::{Diagnostic, ErrorOccurred, NACResult};
use lexer::Lexer;
use parser::Parser;
use sema::Sema;
use session::Session;
use source::file::SourceFileReader;
use target::TargetGen;

pub struct Compiler {
    session: Session,
}

impl Compiler {
    pub fn new() -> Self {
        Self {
            session: Session::new(),
        }
    }

    pub fn session(&self) -> &Session {
        &self.session
    }

    pub fn run_pass<'a, T>(
        &self,
        pass: impl FnOnce() -> Result<T, Diagnostic<'a, ErrorOccurred>>,
    ) -> NACResult<T> {
        let result = pass().map_err(|mut error| error.emit())?;
        self.session.has_errors()?;

        Ok(result)
    }

    pub fn compile_file(&self, context: &CompilerContext, path: &str) -> NACResult<()> {
        let src_file =
            self.run_pass(|| SourceFileReader::source_file_from_path(&self.session, path))?;

        let token_stream = self.run_pass(|| Lexer::tokenize(&self.session, src_file.contents()))?;
        let ast = self.run_pass(|| Parser::parse(&self.session, token_stream))?;

        let ir = self.run_pass(|| Sema::lower_ast(context, ast))?;

        self.run_pass(|| Sema::type_check_ir(context, &ir))?;

        let module = self.run_pass(|| CodeGen::codegen(context, "module", &ast))?;
        self.run_pass(|| TargetGen::compile_module(context, &module))?;

        Ok(())
    }
}

impl Default for Compiler {
    fn default() -> Self {
        Self::new()
    }
}
