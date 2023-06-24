use super::*;
use crate::ast::expr::LiteralKind;

macro_rules! parses_and_matches {
    ($src:literal, $parser_fn:ident, $expected:expr) => {
        let session = Session::new();
        let token_stream = Lexer::tokenize(&session, $src).unwrap();
        let mut parser = Parser::new(&session, token_stream);

        let result = parser.$parser_fn().unwrap();
        assert_eq!(result.kind, $expected);
    };
}

// tests expr:
//      {}
#[test]
fn parser_identifies_block_expr() {
    let src = "{}";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let expr = parser.parse_expr();
    assert!(expr.is_ok());
}

// tests expr:
//      ret 2;
#[test]
fn parser_identifies_ret_expr() {
    let src = "ret 2;";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let expr = parser.parse_expr();
    assert!(expr.is_ok());
}

// tests expr:
//      2
#[test]
fn parser_identifies_integer_literal_expr() {
    let src = "2";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let expr = parser.parse_expr();
    assert!(expr.is_ok());
}

// tests expr:
//      fn
#[test]
fn parser_identifies_invalid_expr() {
    let src = "fn";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let expr = parser.parse_expr();
    assert!(expr.is_err());
}

// tests block:
//      {}
#[test]
fn parser_parses_empty_block() {
    let src = "{}";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let block = parser.parse_block().unwrap();
    assert_eq!(block.stmts.len(), 0);
}

// tests block:
//      { ret 2; }
#[test]
fn parser_parses_block_with_single_stmt() {
    let src = "{ ret 2; }";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let block = parser.parse_block().unwrap();
    assert_eq!(block.stmts.len(), 1);
}

// tests block:
//      {
//          ret 1;
//          ret 2;
//      }
#[test]
fn parser_parses_block_with_multiple_stmts() {
    let src = "{\n\
                        ret 1;\n\
                        ret 2;\n\
                    }";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let block = parser.parse_block().unwrap();
    assert_eq!(block.stmts.len(), 2);
}

// tests block:
//      {
//          fn;
//      }
#[test]
fn parser_ignores_invalid_stmt_in_block_and_diagnoses_error() {
    let src = "{\n\
                        fn;\n\
                     }";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let block = parser.parse_block().unwrap();

    assert!(session.has_errors());
    assert_eq!(session.error_count(), 1);
    assert_eq!(block.stmts.len(), 0);
}

// tests block:
//      {
//          fn;
//          ret 2;
//      }
#[test]
fn parser_recovers_after_invalid_statement_in_block() {
    let src = "{\n\
                        fn;\n\
                        ret 2;\n\
                     }";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let block = parser.parse_block().unwrap();

    assert!(session.has_errors());
    assert_eq!(session.error_count(), 1);
    assert_eq!(block.stmts.len(), 1);
}

// tests block:
//      {
//          fn;
//          ret 2;
//          fn;
//      }
#[test]
fn parser_diagnoses_multiple_invalid_stmts_in_block_after_recovery() {
    let src = "{\n\
                        fn;\n\
                        ret 2;\n\
                        fn;\n\
                     }";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let block = parser.parse_block().unwrap();

    assert!(session.has_errors());
    assert_eq!(session.error_count(), 2);
    assert_eq!(block.stmts.len(), 1);
}

// tests block:
//      {
//          fn;
//          fn;
//      }
#[test]
fn parser_diagnoses_multiple_invalid_stmts_in_block() {
    let src = "{\n\
                        fn;\n\
                        fn;\n\
                     }";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let block = parser.parse_block().unwrap();

    assert!(session.has_errors());
    assert_eq!(session.error_count(), 2);
    assert_eq!(block.stmts.len(), 0);
}

// tests ret expr:
//      ret 2
#[test]
fn parser_parses_ret_expr() {
    let src = "ret 2";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let ret_expr = parser.parse_ret_expr();
    assert!(ret_expr.is_ok());
}

// tests ret expr:
//      ret
#[test]
fn parser_identifies_invalid_ret_expr() {
    let src = "ret";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let ret_expr = parser.parse_ret_expr();
    assert!(ret_expr.is_err());
}

// tests integer literals:
//      2
//      567
//      65536
#[test]
fn parser_parses_integer_literal() {
    parses_and_matches!("2", parse_integer_literal, LiteralKind::Integer(2));
    parses_and_matches!("567", parse_integer_literal, LiteralKind::Integer(567));
    parses_and_matches!("65536", parse_integer_literal, LiteralKind::Integer(65536));
}

// tests expr:
//      fn
#[test]
fn parser_identifies_invalid_integer_literal() {
    let src = "fn";

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();
    let mut parser = Parser::new(&session, token_stream);

    let integer_literal = parser.parse_integer_literal();
    assert!(integer_literal.is_err());
}
