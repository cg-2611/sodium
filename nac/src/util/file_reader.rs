use std::fs;

use crate::errors::{Diagnostic, DiagnosticLevel, Result};

pub fn read_file(path: &str) -> Result<String> {
    match fs::read_to_string(path) {
        Ok(string) => Ok(string),
        Err(e) => Err(Diagnostic::without_range(
            DiagnosticLevel::Fatal,
            format!("could not read contents of file {}: {}", path, e),
        )),
    }
}
