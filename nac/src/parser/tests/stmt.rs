use super::*;

// tests stmt:
//      ret 2;
#[test]
fn parser_identifies_expr_stmt() {
    let src = "ret 2;";

    let session = Session::new();
    let mut parser = initialise_parser_test!(session, src);

    let stmt_result = parser.parse_stmt();

    assert!(stmt_result.is_ok());
    has_errors!(session, 0);
}

// tests expr stmt:
//      ret 2;
#[test]
fn parser_parses_expr_stmt() {
    let src = "ret 2;";

    let session = Session::new();
    let mut parser = initialise_parser_test!(session, src);

    let expr_stmt_result = parser.parse_expr_stmt();

    assert!(expr_stmt_result.is_ok());
    has_errors!(session, 0);
}

// tests expr stmts:
//      ret ;
//      ret 2
#[test]
fn parser_identifies_invalid_expr_stmts() {
    identifies_invalid!("ret ;", parse_expr_stmt);
    identifies_invalid!("ret 2", parse_expr_stmt);
}
