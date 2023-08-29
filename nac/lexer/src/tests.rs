use crate::Lexer;

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
        let sess = session::Session::new();
        let token_stream = Lexer::tokenize(&sess, $src).unwrap();

        match_expected!(token_stream, $expected);

        assert!(sess.has_errors().is_err());
        assert_eq!(sess.error_count(), $count);
    };
}

#[test]
fn lexer_tokenizes_keywords() {
    tokenizes!("fn", token::TokenKind::Keyword(token::Keyword::Fn));
    tokenizes!("ret", token::TokenKind::Keyword(token::Keyword::Ret));
}

#[test]
fn lexer_tokenizes_identifiers() {
    tokenizes!("ident", token::TokenKind::Identifier(String::from("ident")));
    tokenizes!(
        "_ident",
        token::TokenKind::Identifier(String::from("_ident"))
    );
    tokenizes!(
        "ident1",
        token::TokenKind::Identifier(String::from("ident1"))
    );
    tokenizes!(
        "_ident1",
        token::TokenKind::Identifier(String::from("_ident1"))
    );
    tokenizes!(
        "ident_1_",
        token::TokenKind::Identifier(String::from("ident_1_"))
    );
    tokenizes!(
        "_ident_1_",
        token::TokenKind::Identifier(String::from("_ident_1_"))
    );
}

#[test]
fn lexer_tokenizes_integer_literals() {
    tokenizes!("2", token::TokenKind::IntegerLiteral(2));
    tokenizes!("123", token::TokenKind::IntegerLiteral(123));
    tokenizes!("123456789", token::TokenKind::IntegerLiteral(123456789));
}

#[test]
fn lexer_tokenizes_symbols() {
    tokenizes!("{", token::TokenKind::LeftBrace);
    tokenizes!("}", token::TokenKind::RightBrace);
    tokenizes!("(", token::TokenKind::LeftParen);
    tokenizes!(")", token::TokenKind::RightParen);
    tokenizes!(";", token::TokenKind::Semicolon);
    tokenizes!("->", token::TokenKind::Arrow);
}

#[test]
fn lexer_tokenizes_empty_string() {
    tokenizes!("", token::TokenKind::EOF);
}

#[test]
fn lexer_skips_whitespace() {
    tokenizes!("  \n  \t \r   ", token::TokenKind::EOF);
    tokenizes!(
        "  \n\n \rident \t \n ",
        token::TokenKind::Identifier(String::from("ident"))
    );
}

#[test]
fn lexer_tokenizes_multiple_tokens() {
    let src = "ident 2 fn; { -> ) i32";
    let expected_tokens = vec![
        token::TokenKind::Identifier(String::from("ident")),
        token::TokenKind::IntegerLiteral(2),
        token::TokenKind::Keyword(token::Keyword::Fn),
        token::TokenKind::Semicolon,
        token::TokenKind::LeftBrace,
        token::TokenKind::Arrow,
        token::TokenKind::RightParen,
        token::TokenKind::Identifier(String::from("i32")),
        token::TokenKind::EOF,
    ];

    let sess = session::Session::new();
    let token_stream = Lexer::tokenize(&sess, src).unwrap();

    assert!(sess.has_errors().is_ok());
    assert_eq!(sess.error_count(), 0);
    assert_eq!(token_stream.vec().len(), expected_tokens.len());

    match_expected!(token_stream, expected_tokens);
}

#[test]
fn lexer_diagnoses_errors() {
    let expected_tokens = vec![token::TokenKind::Unknown('$')];
    diagnoses!(1, "$", expected_tokens);

    let expected_tokens = vec![
        token::TokenKind::Unknown('$'),
        token::TokenKind::Identifier(String::from("ident")),
    ];
    diagnoses!(1, "$ident", expected_tokens);

    let expected_tokens = vec![
        token::TokenKind::Identifier(String::from("ident")),
        token::TokenKind::Unknown('$'),
        token::TokenKind::Identifier(String::from("i32")),
    ];
    diagnoses!(1, "ident$i32", expected_tokens);

    let expected_tokens = vec![
        token::TokenKind::Keyword(token::Keyword::Fn),
        token::TokenKind::Unknown('$'),
        token::TokenKind::Identifier(String::from("ident")),
        token::TokenKind::Unknown('$'),
        token::TokenKind::Identifier(String::from("i32")),
    ];
    diagnoses!(2, "fn$ident$i32", expected_tokens);
}
