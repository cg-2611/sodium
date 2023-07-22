use super::*;

macro_rules! parse_specific {
    ($session: expr, $parse_fn: ident, $src: expr) => {{
        let token_stream = crate::lexer::Lexer::tokenize(&$session, $src).unwrap();
        let mut parser = crate::parser::Parser::new(&$session, token_stream);
        let result = parser.$parse_fn().unwrap();

        result
    }};
}

macro_rules! initialise_codegen_test {
    ($session: expr, $context:expr) => {{
        let codegen = CodeGen::new(&$session, &$context, "test_module");

        codegen
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

macro_rules! emit_diagnostic {
    ($result: expr) => {
        let _ = $result.map_err(|mut error| error.emit());
    };
}

// tests source file:
//      fn main() -> i32 { ret 0; }
#[test]
fn codegen_generates_module_for_valid_ast() {
    let src = "fn main() -> i32 { ret 0; }";

    let session = Session::new();
    let token_stream = crate::lexer::Lexer::tokenize(&session, src).unwrap();
    let ast = crate::parser::Parser::parse(&session, token_stream).unwrap();

    let context = Context::create();
    let module = CodeGen::codegen(&session, &context, "test_module", &ast);

    assert!(module.is_ok());
    has_errors!(session, 0);
}

// tests source file:
//
#[test]
fn codegen_generates_empty_source_file() {
    let src = "";

    let session = Session::new();
    let source_file = parse_specific!(session, parse_source_file, src);

    let context = Context::create();
    let codegen = initialise_codegen_test!(session, context);

    let sourcefile_result = codegen.walk_source_file(&source_file);

    assert!(sourcefile_result.is_ok());
    has_errors!(session, 0);
}

// tests source file:
//      fn main() -> i32 { ret 0; }
#[test]
fn codegen_generates_source_file() {
    let src = "fn main() -> i32 { ret 0; }";

    let session = Session::new();
    let source_file = parse_specific!(session, parse_source_file, src);

    let context = Context::create();
    let codegen = initialise_codegen_test!(session, context);

    let sourcefile_result = codegen.walk_source_file(&source_file);

    assert!(sourcefile_result.is_ok());
    has_errors!(session, 0);
}

// tests decl:
//      fn main() -> i32 { ret 0; }
#[test]
fn codegen_generates_decl() {
    let src = "fn main() -> i32 { ret 0; }";

    let session = Session::new();
    let decl = parse_specific!(session, parse_decl, src).unwrap();

    let context = Context::create();
    let codegen = initialise_codegen_test!(session, context);

    let decl_result = codegen.walk_decl(&decl);

    assert!(decl_result.is_ok());
    assert!(decl_result.unwrap().is_some());
    has_errors!(session, 0);
}

// tests decl:
//      fn main() -> i32 {}
#[test]
fn codegen_diagnoses_error_for_invalid_decl() {
    let src = "fn main() -> i32 {}";

    let session = Session::new();
    let decl = parse_specific!(session, parse_decl, src).unwrap();

    let context = Context::create();
    let codegen = initialise_codegen_test!(session, context);

    let decl_result = codegen.walk_decl(&decl);

    assert!(decl_result.is_err());
    emit_diagnostic!(decl_result);
    has_errors!(session, 1);
}

// tests fn decl:
//      fn main() -> i32 { ret 0; }
#[test]
fn codegen_generates_fn_decl() {
    let src = "fn main() -> i32 { ret 0; }";

    let session = Session::new();
    let fn_decl = parse_specific!(session, parse_fn_decl, src);

    let context = Context::create();
    let codegen = initialise_codegen_test!(session, context);

    let fn_decl_result = codegen.walk_fn_decl(&fn_decl);

    assert!(fn_decl_result.is_ok());
    assert!(fn_decl_result.unwrap().is_some());
    has_errors!(session, 0);
}

// tests fn decl:
//      fn main() -> i32 {}
#[test]
fn codegen_diagnoses_error_for_invalid_fn_decl() {
    let src = "fn main() -> i32 {}";

    let session = Session::new();
    let fn_decl = parse_specific!(session, parse_fn_decl, src);

    let context = Context::create();
    let codegen = initialise_codegen_test!(session, context);

    let fn_decl_result = codegen.walk_fn_decl(&fn_decl);

    assert!(fn_decl_result.is_err());
    emit_diagnostic!(fn_decl_result);
    has_errors!(session, 1);
}

// tests identifier:
//      name
#[test]
fn codegen_generates_identifier() {
    let src = "name";

    let session = Session::new();
    let identifier = parse_specific!(session, parse_ident, src);

    let context = Context::create();
    let codegen = initialise_codegen_test!(session, context);

    let identifier_result = codegen.walk_ident(&identifier);

    assert!(identifier_result.is_ok());
    has_errors!(session, 0);
}

// tests identifier:
//      i32
#[test]
fn codegen_generates_type() {
    let src = "i32";

    let session = Session::new();
    let ty = parse_specific!(session, parse_type, src);

    let context = Context::create();
    let codegen = initialise_codegen_test!(session, context);

    let type_result = codegen.walk_type(&ty);

    assert!(type_result.is_ok());
    has_errors!(session, 0);
}

// tests block:
//      { ret 0; }
#[test]
fn codegen_generates_block() {
    let src = "{ ret 0; }";

    let session = Session::new();
    let block = parse_specific!(session, parse_block, src);

    let context = Context::create();
    let codegen = initialise_codegen_test!(session, context);

    let block_result = codegen.walk_block(&block);

    assert!(block_result.is_ok());
    assert!(block_result.unwrap().is_some());
    has_errors!(session, 0);
}

// tests block:
//      {}
#[test]
fn codegen_diagnoses_error_for_empty_block() {
    let src = "{}";

    let session = Session::new();
    let block = parse_specific!(session, parse_block, src);

    let context = Context::create();
    let codegen = initialise_codegen_test!(session, context);

    let block_result = codegen.walk_block(&block);

    assert!(block_result.is_err());
    emit_diagnostic!(block_result);
    has_errors!(session, 1);
}

// tests stmt:
//      ret 0;
#[test]
fn codegen_generates_stmt() {
    let src = "ret 0;";

    let session = Session::new();
    let stmt = parse_specific!(session, parse_stmt, src);

    let context = Context::create();
    let codegen = initialise_codegen_test!(session, context);

    let stmt_result = codegen.walk_stmt(&stmt);

    assert!(stmt_result.is_ok());
    assert!(stmt_result.unwrap().is_some());
    has_errors!(session, 0);
}

// tests expr:
//      ret 0
#[test]
fn codegen_generates_expr() {
    let src = "ret 0";

    let session = Session::new();
    let expr = parse_specific!(session, parse_expr, src);

    let context = Context::create();
    let codegen = initialise_codegen_test!(session, context);

    let expr_result = codegen.walk_expr(&expr);

    assert!(expr_result.is_ok());
    assert!(expr_result.unwrap().is_some());
    has_errors!(session, 0);
}

// tests ret expr:
//      ret 0
#[test]
fn codegen_generates_ret_expr() {
    let src = "ret 0";

    let session = Session::new();
    let ret_expr = parse_specific!(session, parse_ret_expr, src);

    let context = Context::create();
    let codegen = initialise_codegen_test!(session, context);

    let ret_expr_result = codegen.walk_ret_expr(&ret_expr);

    assert!(ret_expr_result.is_ok());
    assert!(ret_expr_result.unwrap().is_some());
    has_errors!(session, 0);
}

// tests literal:
//      0
#[test]
fn codegen_generates_literal() {
    let src = "0";

    let session = Session::new();
    let literal = parse_specific!(session, parse_integer_literal, src);

    let context = Context::create();
    let codegen = initialise_codegen_test!(session, context);

    let literal_result = codegen.walk_literal(&literal);

    assert!(literal_result.is_ok());
    assert!(literal_result.unwrap().is_some());
    has_errors!(session, 0);
}
