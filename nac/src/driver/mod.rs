use std::env;

use crate::basic::source_file::SourceFile;

pub fn main() {
    let args: Vec<String> = env::args().collect();

    let file = SourceFile::new(&args[1]);
    compile_file(&file);
}

fn compile_file(file: &SourceFile) {
    println!("{}:\n{}", file.path(), file.contents());
    println!("[nac]: compiled file {}", file.name());
}
