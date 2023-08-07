use std::path::{Path, PathBuf};

use errors::{Diagnostic, ErrorOccurred};
use session::Session;

pub struct SourceFileReader<'a> {
    session: &'a Session,
    path: PathBuf,
}

impl<'a> SourceFileReader<'a> {
    pub fn new(session: &'a Session, path: &str) -> Self {
        Self {
            session,
            path: PathBuf::from(path),
        }
    }

    pub fn source_file_from_path(
        session: &'a Session,
        path: &str,
    ) -> Result<SourceFile, Diagnostic<'a, ErrorOccurred>> {
        let source_file_reader = SourceFileReader::new(session, path);

        if let Some(extension) = source_file_reader.path.extension().and_then(|e| e.to_str()) {
            if extension != "na" {
                let message = format!(
                    "invalid path {:?}: file extension must be .na",
                    source_file_reader.path
                );
                return Err(session.create_error(message));
            }
        } else {
            let message = format!(
                "could not determine extension of path {:?}",
                source_file_reader.path
            );
            return Err(session.create_error(message));
        }

        let file_path = Path::new(path);
        let name = match file_path.file_name().and_then(|n| n.to_str()) {
            Some(name) => name,
            None => {
                let message = format!(
                    "could not determine name of file at path {:?}",
                    source_file_reader.path
                );
                return Err(session.create_error(message));
            }
        };

        let contents = source_file_reader.read_file()?;

        Ok(SourceFile::new(path, name, contents))
    }

    pub fn read_file(&self) -> Result<String, Diagnostic<'a, ErrorOccurred>> {
        match std::fs::read_to_string(&self.path) {
            Ok(string) => Ok(string),
            Err(error) => {
                let message = format!("could not read contents of file {:?}: {}", self.path, error);
                Err(self.session.create_error(message))
            }
        }
    }
}

pub struct SourceFile {
    path: String,
    name: String,
    contents: String,
}

impl SourceFile {
    pub fn new(path: &str, name: &str, contents: String) -> Self {
        Self {
            path: String::from(path),
            name: String::from(name),
            contents,
        }
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
