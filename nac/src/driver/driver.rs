use crate::basic::SourceFile;
use crate::lexer::Lexer;

use std::env;

pub fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() != 2 {
        eprintln!("invalid arguments passed");
        std::process::exit(1);
    }

    let file = SourceFile::new(&args[1]);
    compile_file(&file);
}

fn compile_file(file: &SourceFile) {
    let mut lexer = Lexer::new(file.contents());
    let token_stream = lexer.tokenize();

    println!("[nac]: compiled file {}", file.name());
}
