use errors::{Diagnostic, DiagnosticHandler, ErrorOccurred, NACResult};
use range::Range;
use std::path::{Path, PathBuf};
use symbol::SymbolInterner;

pub struct Session {
    input: Input,
    output: Output,
    diagnostic_handler: DiagnosticHandler,
    symbol_interner: SymbolInterner,
}

impl Session {
    pub fn new(path: &str) -> NACResult<Self> {
        let diagnostic_handler = DiagnosticHandler::new();

        let input = Self::create_input(path, &diagnostic_handler)?;
        let output = Self::create_output(path, &diagnostic_handler)?;

        let sess = Self {
            input,
            output,
            diagnostic_handler,
            symbol_interner: SymbolInterner::new(),
        };

        Ok(sess)
    }

    pub fn create_input(path: &str, diagnostic_handler: &DiagnosticHandler) -> NACResult<Input> {
        let path_buf = PathBuf::from(path);
        if let Some(extension) = path_buf.extension().and_then(|e| e.to_str()) {
            if extension != "na" {
                let message = format!("invalid path {:?}: file extension must be .na", path);
                return Err(diagnostic_handler.create_error(message).emit());
            }
        } else {
            let message = format!(
                "invalid path {:?}: failed to determine file extension",
                path
            );
            return Err(diagnostic_handler.create_error(message).emit());
        }

        let file_path = Path::new(path);
        let name = match file_path.file_name().and_then(|n| n.to_str()) {
            Some(name) => name,
            None => {
                let message = format!("invalid path {:?}: failed to determine file name", path);
                return Err(diagnostic_handler.create_error(message).emit());
            }
        };

        Ok(Input::new(path, name))
    }

    pub fn create_output(path: &str, diagnostic_handler: &DiagnosticHandler) -> NACResult<Output> {
        let path_buf = Path::new(path).with_extension("");
        let path = match path_buf.to_str() {
            Some(path) => path,
            None => {
                let message = format!(
                    "invalid path {:?}: failed to compute output file path",
                    path
                );
                return Err(diagnostic_handler.create_error(message).emit());
            }
        };

        let name = match path_buf.file_name().and_then(|n| n.to_str()) {
            Some(name) => name,
            None => {
                let message = format!(
                    "invalid path {:?}: failed to determine output file name",
                    path
                );
                return Err(diagnostic_handler.create_error(message).emit());
            }
        };

        Ok(Output::new(path, name))
    }

    pub fn input(&self) -> &Input {
        &self.input
    }

    pub fn output(&self) -> &Output {
        &self.output
    }

    pub fn symbol_interner(&self) -> &SymbolInterner {
        &self.symbol_interner
    }

    pub fn create_ranged_error(
        &self,
        message: String,
        range: Range,
    ) -> Diagnostic<'_, ErrorOccurred> {
        self.diagnostic_handler.create_ranged_error(message, range)
    }

    pub fn create_error(&self, message: String) -> Diagnostic<'_, ErrorOccurred> {
        self.diagnostic_handler.create_error(message)
    }

    pub fn error_count(&self) -> usize {
        self.diagnostic_handler.error_count()
    }

    pub fn has_errors(&self) -> NACResult<()> {
        self.diagnostic_handler.has_errors().map_or(Ok(()), Err)
    }
}

pub struct Input {
    path: String,
    name: String,
}

impl Input {
    pub fn new(path: &str, name: &str) -> Self {
        Self {
            path: String::from(path),
            name: String::from(name),
        }
    }

    pub fn path(&self) -> &str {
        &self.path
    }

    pub fn name(&self) -> &str {
        &self.name
    }
}

pub struct Output {
    path: String,
    name: String,
}

impl Output {
    pub fn new(path: &str, name: &str) -> Self {
        Self {
            path: String::from(path),
            name: String::from(name),
        }
    }

    pub fn path(&self) -> &str {
        &self.path
    }

    pub fn name(&self) -> &str {
        &self.name
    }
}
