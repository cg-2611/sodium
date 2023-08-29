pub(crate) use macros::*;

use crate::Parser;

mod decl;
mod expr;
mod macros;
mod stmt;

// tests source file:
//      fn name() -> i32 {}
#[test]
fn parser_produces_ast_from_valid_source_file() {
    let token_kinds = vec![
        token::TokenKind::Keyword(token::Keyword::Fn),
        token::TokenKind::Identifier(String::from("name")),
        token::TokenKind::LeftParen,
        token::TokenKind::RightParen,
        token::TokenKind::Arrow,
        token::TokenKind::Identifier(String::from("i32")),
        token::TokenKind::LeftBrace,
        token::TokenKind::RightBrace,
    ];

    let mut tokens = Vec::new();

    for kind in token_kinds {
        tokens.push(token::Token::new(kind, range::Range::dummy()))
    }

    let token_stream = token::token_stream::TokenStream::from(tokens);

    let sess = session::Session::new();
    let ast = Parser::parse(&sess, token_stream);

    assert!(ast.is_ok());
    has_errors!(sess, 0);
}

// tests source file:
//
#[test]
fn parser_parses_empty_source_file() {
    let tokens = vec![];

    let sess = session::Session::new();
    let mut parser = initialise_parser_test!(sess, tokens);

    let source_file = parser.parse_source_file().unwrap();

    assert_eq!(source_file.decls.len(), 0);
    has_errors!(sess, 0);
}

// tests source file:
//      fn name() -> i32 {}
#[test]
fn parser_parses_source_file_with_single_decl() {
    let tokens = vec![
        token::TokenKind::Keyword(token::Keyword::Fn),
        token::TokenKind::Identifier(String::from("name")),
        token::TokenKind::LeftParen,
        token::TokenKind::RightParen,
        token::TokenKind::Arrow,
        token::TokenKind::Identifier(String::from("i32")),
        token::TokenKind::LeftBrace,
        token::TokenKind::RightBrace,
    ];

    let sess = session::Session::new();
    let mut parser = initialise_parser_test!(sess, tokens);

    let source_file = parser.parse_source_file().unwrap();

    assert_eq!(source_file.decls.len(), 1);
    has_errors!(sess, 0);
}

// tests source file:
//      fn name1() -> i32 {}
//      fn name2() -> i32 [}
#[test]
fn parser_parses_source_file_with_multiple_decls() {
    let tokens = vec![
        token::TokenKind::Keyword(token::Keyword::Fn),
        token::TokenKind::Identifier(String::from("name1")),
        token::TokenKind::LeftParen,
        token::TokenKind::RightParen,
        token::TokenKind::Arrow,
        token::TokenKind::Identifier(String::from("i32")),
        token::TokenKind::LeftBrace,
        token::TokenKind::RightBrace,
        token::TokenKind::Keyword(token::Keyword::Fn),
        token::TokenKind::Identifier(String::from("name2")),
        token::TokenKind::LeftParen,
        token::TokenKind::RightParen,
        token::TokenKind::Arrow,
        token::TokenKind::Identifier(String::from("i32")),
        token::TokenKind::LeftBrace,
        token::TokenKind::RightBrace,
    ];

    let sess = session::Session::new();
    let mut parser = initialise_parser_test!(sess, tokens);

    let source_file = parser.parse_source_file().unwrap();

    assert_eq!(source_file.decls.len(), 2);
    has_errors!(sess, 0);
}

// tests source file:
//      name() -> i32 {}
#[test]
fn parser_ignores_invalid_decl_and_diagnoses_error() {
    let tokens = vec![
        token::TokenKind::Identifier(String::from("name")),
        token::TokenKind::LeftParen,
        token::TokenKind::RightParen,
        token::TokenKind::Arrow,
        token::TokenKind::Identifier(String::from("i32")),
        token::TokenKind::LeftBrace,
        token::TokenKind::RightBrace,
    ];

    let sess = session::Session::new();
    let mut parser = initialise_parser_test!(sess, tokens);

    let source_file = parser.parse_source_file().unwrap();

    assert_eq!(source_file.decls.len(), 0);
    has_errors!(sess, 1);
}

// tests source file:
//      name1() -> i32 {}
//      fn name2() -> i32 {}
#[test]
fn parser_recovers_after_invalid_declaration() {
    let tokens = vec![
        token::TokenKind::Identifier(String::from("name1")),
        token::TokenKind::LeftParen,
        token::TokenKind::RightParen,
        token::TokenKind::Arrow,
        token::TokenKind::Identifier(String::from("i32")),
        token::TokenKind::LeftBrace,
        token::TokenKind::RightBrace,
        token::TokenKind::Keyword(token::Keyword::Fn),
        token::TokenKind::Identifier(String::from("name2")),
        token::TokenKind::LeftParen,
        token::TokenKind::RightParen,
        token::TokenKind::Arrow,
        token::TokenKind::Identifier(String::from("i32")),
        token::TokenKind::LeftBrace,
        token::TokenKind::RightBrace,
    ];

    let sess = session::Session::new();
    let mut parser = initialise_parser_test!(sess, tokens);

    let source_file = parser.parse_source_file().unwrap();

    assert_eq!(source_file.decls.len(), 1);
    has_errors!(sess, 1);
}

// tests source file:
//      name1() -> i32 {}
//      fn name2() -> i32 {}
//      name3() -> i32 {}
#[test]
fn parser_diagnoses_multiple_invalid_decls_after_recovery() {
    let tokens = vec![
        token::TokenKind::Identifier(String::from("name1")),
        token::TokenKind::LeftParen,
        token::TokenKind::RightParen,
        token::TokenKind::Arrow,
        token::TokenKind::Identifier(String::from("i32")),
        token::TokenKind::LeftBrace,
        token::TokenKind::RightBrace,
        token::TokenKind::Keyword(token::Keyword::Fn),
        token::TokenKind::Identifier(String::from("name2")),
        token::TokenKind::LeftParen,
        token::TokenKind::RightParen,
        token::TokenKind::Arrow,
        token::TokenKind::Identifier(String::from("i32")),
        token::TokenKind::LeftBrace,
        token::TokenKind::RightBrace,
        token::TokenKind::Identifier(String::from("name3")),
        token::TokenKind::LeftParen,
        token::TokenKind::RightParen,
        token::TokenKind::Arrow,
        token::TokenKind::Identifier(String::from("i32")),
        token::TokenKind::LeftBrace,
        token::TokenKind::RightBrace,
    ];

    let sess = session::Session::new();
    let mut parser = initialise_parser_test!(sess, tokens);

    let source_file = parser.parse_source_file().unwrap();

    assert_eq!(source_file.decls.len(), 1);
    has_errors!(sess, 2);
}

// tests source file:
//      name() -> i32 {}
//      fn() -> i32 {}
#[test]
fn parser_diagnoses_multiple_invalid_decls() {
    let tokens = vec![
        token::TokenKind::Identifier(String::from("name")),
        token::TokenKind::LeftParen,
        token::TokenKind::RightParen,
        token::TokenKind::Arrow,
        token::TokenKind::Identifier(String::from("i32")),
        token::TokenKind::LeftBrace,
        token::TokenKind::RightBrace,
        token::TokenKind::Keyword(token::Keyword::Fn),
        token::TokenKind::LeftParen,
        token::TokenKind::RightParen,
        token::TokenKind::Arrow,
        token::TokenKind::Identifier(String::from("i32")),
        token::TokenKind::LeftBrace,
        token::TokenKind::RightBrace,
    ];

    let sess = session::Session::new();
    let mut parser = initialise_parser_test!(sess, tokens);

    let source_file = parser.parse_source_file().unwrap();

    assert_eq!(source_file.decls.len(), 0);
    has_errors!(sess, 2);
}

// tests identifier:
//      ident
#[test]
fn parser_parses_identifier() {
    let tokens = vec![token::TokenKind::Identifier(String::from("ident"))];

    let sess = session::Session::new();
    let mut parser = initialise_parser_test!(sess, tokens);

    let ident = parser.parse_ident().unwrap();

    matches_string!(ident.value, "ident");
    has_errors!(sess, 0);
}

// tests type:
//      i32
#[test]
fn parser_parses_type() {
    let tokens = vec![token::TokenKind::Identifier(String::from("i32"))];

    let sess = session::Session::new();
    let mut parser = initialise_parser_test!(sess, tokens);

    let ty = parser.parse_type().unwrap();

    matches_string!(ty.ident.value, "i32");
    has_errors!(sess, 0);
}
