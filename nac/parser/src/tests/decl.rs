use crate::tests::{
    emit_diagnostic, has_errors, identifies_invalid, initialise_parser_test, matches_string,
};

// tests decl:
//      fn name() -> i32 {}
#[test]
fn parser_identifies_fn_decl() {
    let src = "fn name() -> i32 {}";

    let session = session::Session::new();
    let mut parser = initialise_parser_test!(session, src);

    let decl_result = parser.parse_decl();

    assert!(decl_result.is_ok());
    assert!(decl_result.unwrap().is_some());
    has_errors!(session, 0);
}

// tests decl:
//      name() -> i32 {}
#[test]
fn parser_identifies_invalid_decl() {
    let src = "name() -> i32 {}";

    let session = session::Session::new();
    let mut parser = initialise_parser_test!(session, src);

    let decl_result = parser.parse_decl();

    assert!(decl_result.is_err());
    emit_diagnostic!(decl_result);
    has_errors!(session, 1);
}

// tests fn decl:
//      fn name() -> i32 {}
#[test]
fn parser_parses_fn_decl() {
    let src = "fn name() -> i32 {}";

    let session = session::Session::new();
    let mut parser = initialise_parser_test!(session, src);

    let fn_decl = parser.parse_fn_decl().unwrap();

    matches_string!(fn_decl.ident.value, "name");
    matches_string!(fn_decl.ret_type.ident.value, "i32");
    assert_eq!(fn_decl.body.stmts.len(), 0);
    has_errors!(session, 0);
}

// tests fn decls:
//      name() -> i32 {}
//      fn () -> i32 {}
//      fn name -> i32 {}
//      fn name() i32 {}
//      fn name() -> {}
//      fn name() -> i32
#[test]
fn parser_identifies_invalid_fn_decls() {
    identifies_invalid!("name() -> i32 {}", parse_fn_decl);
    identifies_invalid!("fn () -> i32 {}", parse_fn_decl);
    identifies_invalid!("fn name -> i32 {}", parse_fn_decl);
    identifies_invalid!("fn name() i32 {}", parse_fn_decl);
    identifies_invalid!("fn name() -> {}", parse_fn_decl);
    identifies_invalid!("fn name() -> i32", parse_fn_decl);
}
