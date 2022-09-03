#include "sodium/nac/parser/parser.h"

#include <memory>
#include <utility>
#include <vector>

#include "sodium/nac/ast/decl.h"
#include "sodium/nac/ast/type.h"
#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/errors/parser_error.h"
#include "sodium/nac/lexer/token.h"

namespace sodium {

std::unique_ptr<Decl> Parser::parseDecl() {
    switch (token_->kind()) {
        case TokenKind::KEYWORD:
            if (token_->value() == "func") {
                return parseFuncDecl();
            }
            [[fallthrough]];
        default: errorExpected("declaration"); return nullptr;
    }
}

std::unique_ptr<FuncDecl> Parser::parseFuncDecl() {
    std::unique_ptr<FunctionSignature> functionSignature(parseFunctionSignature());
    if (!functionSignature) {
        return nullptr;
    }

    advance(); // advance to expected { to begin block

    std::unique_ptr<Block> functionBody(parseBlock());
    if (!functionBody) {
        return nullptr;
    }

    return std::make_unique<FuncDecl>(std::move(functionSignature), std::move(functionBody));
}

std::unique_ptr<FunctionSignature> Parser::parseFunctionSignature() {
    advance(); // advance to expected identifier

    std::unique_ptr<Identifier> name(parseIdentifier());
    if (!name) {
        errorExpected("function name");
        return nullptr;
    }

    advance(); // advance to expected ( to begin parameter list

    std::unique_ptr<ParameterList> parameters(parseParameterList());
    if (!parameters) {
        return nullptr;
    }

    advance(); // advance to expected ->

    std::unique_ptr<Type> returnType(parseReturnType());
    if (!returnType) {
        return nullptr;
    }

    return std::make_unique<FunctionSignature>(std::move(name), std::move(parameters), std::move(returnType));
}

std::unique_ptr<ParameterList> Parser::parseParameterList() {
    if (!expect(TokenKind::LEFT_PAREN, "expected ( to begin parameter list")) {
        return nullptr;
    }

    advance(); // advance to expected )

    if (!expect(TokenKind::RIGHT_PAREN, "expected ) to end parameter list, pair never closed")) {
        return nullptr;
    }

    return std::make_unique<ParameterList>();
}

std::unique_ptr<Type> Parser::parseReturnType() {
    if (!expect(TokenKind::ARROW, "->")) {
        return nullptr;
    }

    advance(); // advance to expected type

    std::unique_ptr<Type> returnType(parseType());
    if (!returnType) {
        errorExpected("type after ->");
        return nullptr;
    }

    return returnType;
}

} // namespace sodium
