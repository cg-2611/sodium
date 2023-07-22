use super::*;
use tempfile::Builder;

macro_rules! initialise_target_gen_test {
    ($session: expr, $context:expr, $src: expr) => {{
        let token_stream = crate::lexer::Lexer::tokenize(&$session, $src).unwrap();
        let ast = crate::parser::Parser::parse(&$session, token_stream).unwrap();
        let module =
            crate::codegen::CodeGen::codegen(&$session, &$context, "test_module", &ast).unwrap();

        module
    }};
}

macro_rules! has_errors {
    ($session:expr, $count:literal) => {
        match $count {
            0 => assert!($session.has_errors().is_ok()),
            _ => assert!($session.has_errors().is_err()),
        }

        assert_eq!($session.error_count(), $count);
    };
}

macro_rules! emit_diagnostic {
    ($result: expr) => {
        let _ = $result.map_err(|mut error| error.emit());
    };
}

#[test]
fn target_gen_creates_target_machine() {
    let session = Session::new();
    let target_machine = TargetGen::create_target_machine(&session);

    assert!(target_machine.is_ok());
    has_errors!(session, 0);
}

#[test]
fn target_gen_creates_writes_to_object_file() {
    let src = "fn main() -> i32 { ret 0; }";

    let session = Session::new();
    let context = crate::llvm::Context::create();
    let module = initialise_target_gen_test!(session, context, src);

    let target_gen = TargetGen::new(&session, &module).expect("failed to create target gen");

    let tempdir = Builder::new().tempdir().expect("failed to create tempdir");

    let tempdir_path = tempdir
        .path()
        .to_str()
        .expect("failed to convert test tempdir path to str");

    let file_path = std::path::Path::new(tempdir_path)
        .join("test_object_file.o")
        .into_os_string()
        .into_string()
        .expect("failed to convert test file path to string");

    let object_file_result = target_gen.write_object_file(file_path.as_str());

    assert!(object_file_result.is_ok());
    has_errors!(session, 0);

    let object_file_path = std::path::Path::new(file_path.as_str());
    assert!(object_file_path.exists());
    assert!(object_file_path.metadata().unwrap().len() > 0);
}

#[test]
fn target_gen_diagnoses_error_on_linker_failure() {
    let src = "fn main() -> i32 { ret 0; }";

    let session = Session::new();
    let context = crate::llvm::Context::create();
    let module = initialise_target_gen_test!(session, context, src);

    let target_gen = TargetGen::new(&session, &module).expect("failed to create target gen");

    let link_result = target_gen.link_executable("does_not_exist.o");

    assert!(link_result.is_err());
    emit_diagnostic!(link_result);
    has_errors!(session, 1);
}
