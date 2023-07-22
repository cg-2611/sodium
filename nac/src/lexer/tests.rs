use super::*;

macro_rules! tokenizes {
    ($src:literal, $kind:expr) => {
        let token = Lexer::new($src).next_token();
        assert_eq!(token.kind, $kind);
    };
}

macro_rules! match_expected {
    ($tokens:expr, $expected:expr) => {
        let test_tokens = $tokens.vec().iter().zip($expected.iter());
        for (token, expected_token) in test_tokens {
            assert_eq!(&token.kind, expected_token);
        }
    };
}

macro_rules! diagnoses {
    ($count:literal, $src:literal, $expected:expr) => {
        let session = Session::new();
        let token_stream = Lexer::tokenize(&session, $src).unwrap();

        match_expected!(token_stream, $expected);

        assert!(session.has_errors().is_err());
        assert_eq!(session.error_count(), $count);
    };
}

#[test]
fn lexer_tokenizes_keywords() {
    tokenizes!("fn", TokenKind::Keyword(Keyword::Fn));
    tokenizes!("ret", TokenKind::Keyword(Keyword::Ret));
}

#[test]
fn lexer_tokenizes_identifiers() {
    tokenizes!("ident", TokenKind::Identifier(String::from("ident")));
    tokenizes!("_ident", TokenKind::Identifier(String::from("_ident")));
    tokenizes!("ident1", TokenKind::Identifier(String::from("ident1")));
    tokenizes!("_ident1", TokenKind::Identifier(String::from("_ident1")));
    tokenizes!("ident_1_", TokenKind::Identifier(String::from("ident_1_")));
    tokenizes!(
        "_ident_1_",
        TokenKind::Identifier(String::from("_ident_1_"))
    );
}

#[test]
fn lexer_tokenizes_integer_literals() {
    tokenizes!("2", TokenKind::IntegerLiteral(2));
    tokenizes!("123", TokenKind::IntegerLiteral(123));
    tokenizes!("123456789", TokenKind::IntegerLiteral(123456789));
}

#[test]
fn lexer_tokenizes_symbols() {
    tokenizes!("{", TokenKind::LeftBrace);
    tokenizes!("}", TokenKind::RightBrace);
    tokenizes!("(", TokenKind::LeftParen);
    tokenizes!(")", TokenKind::RightParen);
    tokenizes!(";", TokenKind::Semicolon);
    tokenizes!("->", TokenKind::Arrow);
}

#[test]
fn lexer_tokenizes_empty_string() {
    tokenizes!("", TokenKind::EOF);
}

#[test]
fn lexer_skips_whitespace() {
    tokenizes!("  \n  \t \r   ", TokenKind::EOF);
    tokenizes!(
        "  \n\n \rident \t \n ",
        TokenKind::Identifier(String::from("ident"))
    );
}

#[test]
fn lexer_tokenizes_multiple_tokens() {
    let src = "ident 2 fn; { -> ) i32";
    let expected_tokens = vec![
        TokenKind::Identifier(String::from("ident")),
        TokenKind::IntegerLiteral(2),
        TokenKind::Keyword(Keyword::Fn),
        TokenKind::Semicolon,
        TokenKind::LeftBrace,
        TokenKind::Arrow,
        TokenKind::RightParen,
        TokenKind::Identifier(String::from("i32")),
        TokenKind::EOF,
    ];

    let session = Session::new();
    let token_stream = Lexer::tokenize(&session, src).unwrap();

    assert!(session.has_errors().is_ok());
    assert_eq!(session.error_count(), 0);
    assert_eq!(token_stream.vec().len(), expected_tokens.len());

    match_expected!(token_stream, expected_tokens);
}

#[test]
fn lexer_diagnoses_errors() {
    let expected_tokens = vec![TokenKind::Unknown('$')];
    diagnoses!(1, "$", expected_tokens);

    let expected_tokens = vec![
        TokenKind::Unknown('$'),
        TokenKind::Identifier(String::from("ident")),
    ];
    diagnoses!(1, "$ident", expected_tokens);

    let expected_tokens = vec![
        TokenKind::Identifier(String::from("ident")),
        TokenKind::Unknown('$'),
        TokenKind::Identifier(String::from("i32")),
    ];
    diagnoses!(1, "ident$i32", expected_tokens);

    let expected_tokens = vec![
        TokenKind::Keyword(Keyword::Fn),
        TokenKind::Unknown('$'),
        TokenKind::Identifier(String::from("ident")),
        TokenKind::Unknown('$'),
        TokenKind::Identifier(String::from("i32")),
    ];
    diagnoses!(2, "fn$ident$i32", expected_tokens);
}
