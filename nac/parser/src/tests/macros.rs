macro_rules! initialise_parser_test {
    ($session: expr, $src:expr) => {{
        let token_stream = lexer::Lexer::tokenize(&$session, $src).unwrap();
        let parser = crate::Parser::new(&$session, token_stream);

        parser
    }};
}

macro_rules! has_errors {
    ($session:expr, $count:literal) => {
        match $count {
            0 => assert!($session.has_errors().is_ok()),
            _ => assert!($session.has_errors().is_err()),
        }

        assert_eq!($session.error_count(), $count);
    };
}

macro_rules! matches_string {
    ($actual: expr, $expected: literal) => {
        assert_eq!($actual, String::from($expected));
    };
}

macro_rules! emit_diagnostic {
    ($result: expr) => {
        let _ = $result.map_err(|mut error| error.emit());
    };
}

macro_rules! identifies_invalid {
    ($src:literal, $parser_fn:ident) => {
        let src = $src;

        let session = session::Session::new();
        let mut parser = initialise_parser_test!(session, src);

        let result = parser.$parser_fn();
        assert!(result.is_err());

        crate::tests::emit_diagnostic!(result);
    };
}

pub(crate) use emit_diagnostic;
pub(crate) use has_errors;
pub(crate) use identifies_invalid;
pub(crate) use initialise_parser_test;
pub(crate) use matches_string;
