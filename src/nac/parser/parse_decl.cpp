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
    std::unique_ptr<FuncSignature> signature(parse_func_signature());
    if (!signature) {
        return nullptr;
    }

    std::unique_ptr<Block> body(parse_block());
    if (!body) {
        return nullptr;
    }

    return std::make_unique<FuncDecl>(std::move(signature), std::move(body),
                                      signature->range().start().to(body->range().end()));
}

std::unique_ptr<FuncSignature> Parser::parse_func_signature() {
    auto func_keyword_location = token_.range().start();

    if (!expect(TokenKind::KEYWORD_FUNC, ParserErrorKind::EXPECTED_KEYWORD_FUNC)) {
        return nullptr;
    }

    std::unique_ptr<Identifier> name(parse_identifier());
    if (!name) {
        return nullptr;
    }

    std::unique_ptr<ParameterList> parameters(parse_parameter_list());
    if (!parameters) {
        return nullptr;
    }

    std::unique_ptr<Type> return_type(parse_return_type());
    if (!return_type) {
        return nullptr;
    }

    return std::make_unique<FuncSignature>(std::move(name), std::move(parameters), std::move(return_type),
                                           func_keyword_location.to(return_type->range().end()));
}

std::unique_ptr<ParameterList> Parser::parse_parameter_list() {
    auto left_paren_location = token_.range().start();

    if (!expect(TokenKind::LEFT_PAREN, ParserErrorKind::EXPECTED_LEFT_PAREN_PARAMETER_LIST)) {
        return nullptr;
    }

    auto right_paren_location = token_.range().end();

    if (!expect(TokenKind::RIGHT_PAREN, ParserErrorKind::EXPECTED_RIGHT_PAREN_PARAMETER_LIST)) {
        return nullptr;
    }

    return std::make_unique<ParameterList>(left_paren_location.to(right_paren_location));
}

std::unique_ptr<Type> Parser::parse_return_type() {
    if (!expect(TokenKind::ARROW, ParserErrorKind::EXPECTED_ARROW)) {
        return nullptr;
    }

    std::unique_ptr<Type> return_type(parse_type());
    if (!return_type) {
        return nullptr;
    }

    return return_type;
}

} // namespace sodium
