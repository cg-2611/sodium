use crate::tests::{has_errors, identifies_invalid, initialise_parser_test};

// tests stmt:
//      ret 2;
#[test]
fn parser_identifies_expr_stmt() {
    let tokens = vec![
        token::TokenKind::Keyword(token::Keyword::Ret),
        token::TokenKind::IntegerLiteral(2),
        token::TokenKind::Semicolon,
    ];

    let session = session::Session::new();
    let mut parser = initialise_parser_test!(session, tokens);

    let stmt_result = parser.parse_stmt();

    assert!(stmt_result.is_ok());
    has_errors!(session, 0);
}

// tests expr stmt:
//      ret 2;
#[test]
fn parser_parses_expr_stmt() {
    let tokens = vec![
        token::TokenKind::Keyword(token::Keyword::Ret),
        token::TokenKind::IntegerLiteral(2),
        token::TokenKind::Semicolon,
    ];

    let session = session::Session::new();
    let mut parser = initialise_parser_test!(session, tokens);

    let expr_stmt_result = parser.parse_expr_stmt();

    assert!(expr_stmt_result.is_ok());
    has_errors!(session, 0);
}

// tests expr stmts:
//      ret ;
//      ret 2
#[test]
fn parser_identifies_invalid_expr_stmts() {
    let tokens = vec![
        token::TokenKind::Keyword(token::Keyword::Ret),
        token::TokenKind::Semicolon,
    ];
    identifies_invalid!(tokens, parse_expr_stmt);

    let tokens = vec![
        token::TokenKind::Keyword(token::Keyword::Ret),
        token::TokenKind::IntegerLiteral(2),
    ];
    identifies_invalid!(tokens, parse_expr_stmt);
}
