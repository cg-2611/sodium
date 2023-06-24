use std::path::Path;

use crate::errors::{Diagnostic, DiagnosticLevel, Result};
use crate::util;

pub struct SourceFile {
    path: String,
    name: String,
    contents: String,
}

impl SourceFile {
    pub fn from(path: &str) -> Result<Self> {
        let file_path = Path::new(path);

        if let Some(extension) = file_path.extension().and_then(|e| e.to_str()) {
            if extension != "na" {
                return Err(Diagnostic::new(
                    DiagnosticLevel::Fatal,
                    format!("invalid path {}: file extension must be .na", path),
                ));
            }
        } else {
            return Err(Diagnostic::new(
                DiagnosticLevel::Fatal,
                format!("could not determine extension of file {}", path),
            ));
        }

        let name = match file_path.file_name().and_then(|n| n.to_str()) {
            Some(name) => name,
            None => {
                return Err(Diagnostic::new(
                    DiagnosticLevel::Fatal,
                    format!("could not determine name of file {}", path),
                ))
            }
        };

        let contents = util::read_file(path)?;

        Ok(Self {
            path: String::from(path),
            name: String::from(name),
            contents,
        })
    }

    pub fn path(&self) -> &String {
        &self.path
    }

    pub fn name(&self) -> &String {
        &self.name
    }

    pub fn contents(&self) -> &String {
        &self.contents
    }
}
