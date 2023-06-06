use crate::errors::Diagnostic;

#[derive(Default)]
pub struct DiagnosticHandler {
    diagnostics: Vec<Diagnostic>,
    error_count: u32,
}

impl DiagnosticHandler {
    pub fn report(&mut self, diagnostic: Diagnostic) {
        self.diagnostics.push(diagnostic);
        self.error_count += 1;
    }

    pub fn emit(&self) {
        self.diagnostics.iter().for_each(|diagnostic| {
            eprintln!("{}", diagnostic);
        });
    }

    pub fn diagnostic_count(&self) -> usize {
        self.diagnostics.len()
    }

    pub fn error_count(&self) -> u32 {
        self.error_count
    }

    pub fn has_errors(&self) -> bool {
        self.error_count() > 0
    }

    pub fn diagnostics(&self) -> &Vec<Diagnostic> {
        &self.diagnostics
    }
}
