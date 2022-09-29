#include "sodium/nac/parser/parser.h"

#include <memory>
#include <utility>

#include "sodium/nac/ast/decl.h"
#include "sodium/nac/ast/identifier.h"
#include "sodium/nac/ast/type.h"
#include "sodium/nac/basic/source_range.h"
#include "sodium/nac/parser/parser_diagnostics.h"
#include "sodium/nac/token/token.h"

namespace sodium {

std::unique_ptr<Decl> Parser::parse_decl() {
    switch (token_.kind()) {
        case TokenKind::KEYWORD_FUNC: return parse_func_decl();
        default: error_expected(ParserErrorKind::EXPECTED_DECLARATION); return nullptr;
    }
}

std::unique_ptr<FuncDecl> Parser::parse_func_decl() {
    auto func_keyword_location = token_.range().start();

    if (!expect(TokenKind::KEYWORD_FUNC, ParserErrorKind::EXPECTED_KEYWORD_FUNC)) {
        return nullptr;
    }

    auto name = parse_identifier();
    if (!name) {
        return nullptr;
    }

    if (!expect(TokenKind::LEFT_PAREN, ParserErrorKind::EXPECTED_LEFT_PAREN_PARAMETER_LIST)) {
        return nullptr;
    }

    if (!expect(TokenKind::RIGHT_PAREN, ParserErrorKind::EXPECTED_RIGHT_PAREN_PARAMETER_LIST)) {
        return nullptr;
    }

    auto return_type = parse_return_type();
    if (!return_type) {
        return nullptr;
    }

    auto body = parse_block();
    if (!body) {
        return nullptr;
    }

    return std::make_unique<FuncDecl>(std::move(name), std::move(return_type), std::move(body),
                                      func_keyword_location.to(body->range().end()));
}

std::unique_ptr<Type> Parser::parse_return_type() {
    if (!expect(TokenKind::ARROW, ParserErrorKind::EXPECTED_ARROW)) {
        return nullptr;
    }

    auto return_type = parse_type();
    if (!return_type) {
        return nullptr;
    }

    return return_type;
}

} // namespace sodium
