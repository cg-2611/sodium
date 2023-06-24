use super::*;

// tests stmt:
//      ret 2;
#[test]
fn parser_identifies_expr_stmt() {
    let src = "ret 2;";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let stmt = parser.parse_stmt();
    assert!(stmt.is_ok());
}
