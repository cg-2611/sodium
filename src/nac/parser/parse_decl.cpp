#include "sodium/nac/parser/parser.h"

#include <memory>
#include <utility>
#include <vector>

#include "sodium/nac/ast/decl.h"
#include "sodium/nac/ast/type.h"
#include "sodium/nac/lexer/token.h"

namespace sodium {

// Declaration = FunctionDeclaration ;
std::unique_ptr<Decl> Parser::parseDecl() {
    std::unique_ptr<Decl> decl(nullptr);

    if (token_->kind() == TokenKind::KEYWORD && token_->value() == "func") {
        decl = parseFuncDecl();
    }

    // expected a declaration

    return decl;
}

// FunctionDeclaration = func FunctionSignature Block ;
std::unique_ptr<FuncDecl> Parser::parseFuncDecl() {
    std::unique_ptr<FunctionSignature> functionSignature(parseFunctionSignature());

    advance(); // advance to expected block

    std::unique_ptr<Block> functionBody(parseBlock());

    return std::make_unique<FuncDecl>(std::move(functionSignature), std::move(functionBody));
}

// FunctionSignature = Identifier ParameterList ReturnType ;
std::unique_ptr<FunctionSignature> Parser::parseFunctionSignature() {
    advance(); // advance to expected identifier
    std::unique_ptr<Identifier> functionName(parseIdentifier());

    advance(); // advance to expected parameter list
    std::unique_ptr<ParameterList> functionParameters(parseParameterList());

    advance(); // advance to expected return type
    std::unique_ptr<Type> functionReturnType(parseReturnType());

    return std::make_unique<FunctionSignature>(std::move(functionName), std::move(functionParameters),
                                               std::move(functionReturnType));
}

// ParameterList = Parameter Parameter* ;
std::unique_ptr<ParameterList> Parser::parseParameterList() {
    if (token_->kind() != TokenKind::LEFT_PAREN) {
        // expected (
    }

    advance(); // advance to the ) token

    if (token_->kind() != TokenKind::RIGHT_PAREN) {
        // expected a )
    }

    return std::make_unique<ParameterList>();
}

// ReturnType = -> Type ;
std::unique_ptr<Type> Parser::parseReturnType() {
    if (token_->kind() != TokenKind::ARROW) {
        // expected arrow
    }

    advance(); // advance to expected type

    return parseType();
}

} // namespace sodium
