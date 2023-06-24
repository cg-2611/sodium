use super::*;

macro_rules! identifies_invalid {
    ($src:literal, $parser_fn:ident) => {
        let session = Session::new();
        let token_stream = Lexer::tokenize(&session, $src).unwrap();
        let mut parser = Parser::new(&session, token_stream);

        let result = parser.$parser_fn();
        assert!(result.is_err());
    };
}

// tests decl:
//      fn name() -> i32 {}
#[test]
fn parser_identifies_fn_decl() {
    let src = "fn name() -> i32 {}";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let decl = parser.parse_decl().unwrap();

    assert!(decl.is_some());
}

// tests decl:
//      name() -> i32 {}
#[test]
fn parser_identifies_invalid_decl() {
    let src = "name() -> i32 {}";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let decl = parser.parse_decl();
    assert!(decl.is_err());
}

// tests fn decl:
//      fn name() -> i32 {}
#[test]
fn parser_parses_fn_decl() {
    let src = "fn name() -> i32 {}";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let fn_decl = parser.parse_fn_decl().unwrap();

    assert_eq!(fn_decl.ident.value, String::from("name"));
    assert_eq!(fn_decl.ret_type.ident.value, String::from("i32"));
    assert_eq!(fn_decl.body.stmts.len(), 0);
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
