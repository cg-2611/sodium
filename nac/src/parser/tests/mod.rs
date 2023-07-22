use super::*;

mod decl;
mod expr;
mod macros;
mod stmt;

pub(crate) use macros::*;

// tests source file:
//      fn name() -> i32 {}
#[test]
fn parser_produces_ast_from_valid_source_file() {
    let src = "fn name() -> i32 {}";

    let session = Session::new();
    let token_stream = crate::lexer::Lexer::tokenize(&session, src).unwrap();
    let ast = Parser::parse(&session, token_stream);

    assert!(ast.is_ok());
    has_errors!(session, 0);
}

// tests source file:
//
#[test]
fn parser_parses_empty_source_file() {
    let src = "";

    let session = Session::new();
    let mut parser = initialise_parser_test!(session, src);

    let source_file = parser.parse_source_file().unwrap();

    assert_eq!(source_file.decls.len(), 0);
    has_errors!(session, 0);
}

// tests source file:
//      fn name() -> i32 {}
#[test]
fn parser_parses_source_file_with_single_decl() {
    let src = "fn name() -> i32 {}";

    let session = Session::new();
    let mut parser = initialise_parser_test!(session, src);

    let source_file = parser.parse_source_file().unwrap();

    assert_eq!(source_file.decls.len(), 1);
    has_errors!(session, 0);
}

// tests source file:
//      fn name1() -> i32 {}
//      fn name2() -> i32 [}
#[test]
fn parser_parses_source_file_with_multiple_decls() {
    let src = "fn name1() -> i32 {}\n\
                    fn name2() -> i32 {}";

    let session = Session::new();
    let mut parser = initialise_parser_test!(session, src);

    let source_file = parser.parse_source_file().unwrap();

    assert_eq!(source_file.decls.len(), 2);
    has_errors!(session, 0);
}

// tests source file:
//      name() -> i32 {}
#[test]
fn parser_ignores_invalid_decl_and_diagnoses_error() {
    let src = "name() -> i32 {}";

    let session = Session::new();
    let mut parser = initialise_parser_test!(session, src);

    let source_file = parser.parse_source_file().unwrap();

    assert_eq!(source_file.decls.len(), 0);
    has_errors!(session, 1);
}

// tests source file:
//      name1() -> i32 {}
//      fn name2() -> i32 {}
#[test]
fn parser_recovers_after_invalid_declaration() {
    let src = "name1() -> i32 {}\n\
                    fn name2() -> i32 {}";

    let session = Session::new();
    let mut parser = initialise_parser_test!(session, src);

    let source_file = parser.parse_source_file().unwrap();

    assert_eq!(source_file.decls.len(), 1);
    has_errors!(session, 1);
}

// tests source file:
//      name1() -> i32 {}
//      fn name2() -> i32 {}
//      name3() -> i32 {}
#[test]
fn parser_diagnoses_multiple_invalid_decls_after_recovery() {
    let src = "name1() -> i32 {}\n\
                    fn name2() -> i32 {}\n\
                    name3() -> i32 {}";

    let session = Session::new();
    let mut parser = initialise_parser_test!(session, src);

    let source_file = parser.parse_source_file().unwrap();

    assert_eq!(source_file.decls.len(), 1);
    has_errors!(session, 2);
}

// tests source file:
//      name() -> i32 {}
//      fn() -> i32 {}
#[test]
fn parser_diagnoses_multiple_invalid_decls() {
    let src = "name() -> i32 {}\n\
                    fn() -> i32 {}";

    let session = Session::new();
    let mut parser = initialise_parser_test!(session, src);

    let source_file = parser.parse_source_file().unwrap();

    assert_eq!(source_file.decls.len(), 0);
    has_errors!(session, 2);
}

// tests identifier:
//      ident
#[test]
fn parser_parses_identifier() {
    let src = "ident";

    let session = Session::new();
    let mut parser = initialise_parser_test!(session, src);

    let ident = parser.parse_ident().unwrap();

    matches_string!(ident.value, "ident");
    has_errors!(session, 0);
}

// tests type:
//      i32
#[test]
fn parser_parses_type() {
    let src = "i32";

    let session = Session::new();
    let mut parser = initialise_parser_test!(session, src);

    let ty = parser.parse_type().unwrap();

    matches_string!(ty.ident.value, "i32");
    has_errors!(session, 0);
}
