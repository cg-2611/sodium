use super::*;
use crate::lexer::Lexer;

mod decl;
mod expr;
mod stmt;

// tests source file:
//
#[test]
fn parser_parses_empty_source_file() {
    let src = "";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let source_file = parser.parse_source_file().unwrap();

    assert!(!session.has_errors());
    assert_eq!(source_file.decls.len(), 0);
}

// tests source file:
//      fn name() -> i32 {}
#[test]
fn parser_parses_source_file_with_single_decl() {
    let src = "fn name() -> i32 {}";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let source_file = parser.parse_source_file().unwrap();

    assert!(!session.has_errors());
    assert_eq!(source_file.decls.len(), 1);
}

// tests source file:
//      fn name1() -> i32 {}
//      fn name2() -> i32 [}
#[test]
fn parser_parses_source_file_with_multiple_decls() {
    let src = "fn name1() -> i32 {}\n\
                    fn name2() -> i32 {}";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let source_file = parser.parse_source_file().unwrap();

    assert!(!session.has_errors());
    assert_eq!(source_file.decls.len(), 2);
}

// tests source file:
//      name() -> i32 {}
#[test]
fn parser_ignores_invalid_decl_and_diagnoses_error() {
    let src = "name() -> i32 {}";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let source_file = parser.parse_source_file().unwrap();

    assert!(session.has_errors());
    assert_eq!(session.error_count(), 1);
    assert_eq!(source_file.decls.len(), 0);
}

// tests source file:
//      name1() -> i32 {}
//      fn name2() -> i32 {}
#[test]
fn parser_recovers_after_invalid_declaration() {
    let src = "name1() -> i32 {}\n\
                    fn name2() -> i32 {}";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let source_file = parser.parse_source_file().unwrap();

    assert!(session.has_errors());
    assert_eq!(session.error_count(), 1);
    assert_eq!(source_file.decls.len(), 1);
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
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let source_file = parser.parse_source_file().unwrap();

    assert!(session.has_errors());
    assert_eq!(session.error_count(), 2);
    assert_eq!(source_file.decls.len(), 1);
}

// tests source file:
//      name() -> i32 {}
//      fn() -> i32 {}
#[test]
fn parser_diagnoses_multiple_invalid_decls() {
    let src = "name() -> i32 {}\n\
                    fn() -> i32 {}";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let source_file = parser.parse_source_file().unwrap();

    assert!(session.has_errors());
    assert_eq!(session.error_count(), 2);
    assert_eq!(source_file.decls.len(), 0);
}

// tests identifier:
//      ident
#[test]
fn parser_parses_identifier() {
    let src = "ident";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let ident = parser.parse_identifier().unwrap();
    assert_eq!(ident.value, String::from("ident"));
}

// tests type:
//      i32
#[test]
fn parser_parses_type() {
    let src = "i32";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let ty = parser.parse_type().unwrap();
    assert_eq!(ty.ident.value, String::from("i32"));
}
