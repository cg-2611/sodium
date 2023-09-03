use errors::{Diagnostic, ErrorOccurred};
use session::Session;

pub struct SourceFileReader<'a> {
    sess: &'a Session,
    path: &'a str,
}

impl<'a> SourceFileReader<'a> {
    pub fn new(sess: &'a Session, path: &'a str) -> Self {
        Self { sess, path }
    }

    pub fn source_file_from_path(
        sess: &'a Session,
        path: &'a str,
    ) -> Result<SourceFile, Diagnostic<'a, ErrorOccurred>> {
        let source_file_reader = SourceFileReader::new(sess, path);
        let contents = source_file_reader.read_file()?;
        Ok(SourceFile::new(path, contents))
    }

    pub fn read_file(&self) -> Result<String, Diagnostic<'a, ErrorOccurred>> {
        match std::fs::read_to_string(self.path) {
            Ok(string) => Ok(string),
            Err(error) => {
                let message = format!("failed to read contents of file {:?}: {}", self.path, error);
                Err(self.sess.create_error(message))
            }
        }
    }
}

pub struct SourceFile {
    path: String,
    contents: String,
}

impl SourceFile {
    pub fn new(path: &str, contents: String) -> Self {
        Self {
            path: String::from(path),
            contents,
        }
    }

    pub fn path(&self) -> &str {
        &self.path
    }

    pub fn contents(&self) -> &str {
        &self.contents
    }
}
