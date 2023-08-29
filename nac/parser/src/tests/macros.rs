macro_rules! initialise_parser_test {
    ($sess: expr, $token_kinds: expr) => {{
        let mut tokens = Vec::new();

        for kind in $token_kinds {
            tokens.push(token::Token::new(kind, range::Range::dummy()))
        }

        let token_stream = token::token_stream::TokenStream::from(tokens);
        let parser = crate::Parser::new(&$sess, token_stream);

        parser
    }};
}

macro_rules! has_errors {
    ($sess:expr, $count:literal) => {
        match $count {
            0 => assert!($sess.has_errors().is_ok()),
            _ => assert!($sess.has_errors().is_err()),
        }

        assert_eq!($sess.error_count(), $count);
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
    ($tokens:expr, $parser_fn:ident) => {
        let sess = session::Session::new();
        let mut parser = initialise_parser_test!(sess, $tokens);

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
