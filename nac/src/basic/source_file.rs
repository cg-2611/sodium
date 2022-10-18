use std::{ffi::OsStr, path::Path};

use crate::util::file_reader;

pub struct SourceFile {
    path: String,
    name: String,
    contents: String,
}

impl SourceFile {
    pub fn new(path: &str) -> Self {
        let file_path = Path::new(path);

        let name = file_path
            .file_name()
            .unwrap_or(OsStr::new(path))
            .to_str()
            .unwrap_or(path);

        let file_contents = match file_reader::read_file(path) {
            Some(contents) => contents,
            None => String::from(""),
        };

        Self {
            path: String::from(path),
            name: String::from(name),
            contents: file_contents,
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
