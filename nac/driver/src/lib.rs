use compiler::Compiler;
use context::CompilerContext;
use errors::NACResult;

pub mod compiler;

const EXIT_SUCCESS: i32 = 0;
const EXIT_FAILURE: i32 = 1;

fn catch_unwind_with_exit_code(f: impl FnOnce() -> NACResult<()>) -> i32 {
    let result = f();
    match result {
        Ok(_) => EXIT_SUCCESS,
        Err(_) => EXIT_FAILURE,
    }
}

pub fn main() {
    let exit_code = catch_unwind_with_exit_code(|| {
        let compiler = Compiler::default();
        let context = CompilerContext::new(compiler.session());

        let args: Vec<String> = std::env::args().collect();
        if args.len() < 2 {
            return Err(compiler
                .session()
                .create_error("invalid arguments passed".to_string())
                .emit());
        }

        compiler.compile_file(&context, &args[1])
    });

    std::process::exit(exit_code);
}