#include "sodium/nac/parser/parser.h"

#include <memory>
#include <utility>
#include <vector>

#include "sodium/nac/ast/decl.h"
#include "sodium/nac/ast/parameter_list.h"
#include "sodium/nac/ast/type.h"
#include "sodium/nac/lexer/token.h"

namespace sodium {

// Declaration = FunctionDeclaration ;
std::unique_ptr<Decl> Parser::parseDecl() {
    std::unique_ptr<Decl> decl(nullptr);

    if (token_->kind() == TokenKind::KEYWORD && token_->value() == "func") {
        decl = parseFuncDecl();
    }

    // error, expected declaration, did not receive one

    return decl;
}

// FunctionDeclaration = func FunctionSignature FunctionBody ;
// FunctionBody = Block ;
std::unique_ptr<FuncDecl> Parser::parseFuncDecl() {
    std::unique_ptr<Signature> functionSignature(parseSignature());

    advance(); // advance to expected block

    std::unique_ptr<Block> functionBody(parseBlock());

    return std::make_unique<FuncDecl>(std::move(functionSignature), std::move(functionBody));
}

// FunctionSignature = FunctionName FunctionParameters FunctionReturnType ;
// FunctionName = Identifier ;
// FunctionParameters = ( ParameterList? ) ;
std::unique_ptr<Signature> Parser::parseSignature() {
    advance(); // advance to expected identifier
    std::unique_ptr<Identifier> functionName(parseIdentifier());

    advance(); // advance to expected parameter list
    std::unique_ptr<ParameterList> functionParameters(parseParameterList());

    advance(); // advance to expected return type
    std::unique_ptr<Type> functionReturnType(parseReturnType());

    return std::make_unique<Signature>(std::move(functionName), std::move(functionParameters),
                                       std::move(functionReturnType));
}

// ParameterList = Parameter Parameter* ;
std::unique_ptr<ParameterList> Parser::parseParameterList() {
    if (token_->kind() != TokenKind::LEFT_PAREN) {
        // error, expected a parameter list, but did not receive one
    }

    std::vector<std::unique_ptr<Parameter>> parameters{};

    // parse parameters until we reach the end of the parameter list
    while (nextToken()->kind() != TokenKind::RIGHT_PAREN) {
        advance(); // advance to expected parameter
        parameters.push_back(parseParameter());
    }

    advance(); // advance to the ) token

    // TODO: error if bracket pair never closed
    // TODO: maximum number of parameters

    return std::make_unique<ParameterList>(std::move(parameters));
}

// Parameter = Identifier ;
std::unique_ptr<Parameter> Parser::parseParameter() {
    return std::make_unique<Parameter>(parseIdentifier());
}

// FunctionReturnType = -> Type ;
std::unique_ptr<Type> Parser::parseReturnType() {
    if (token_->kind() != TokenKind::ARROW) {
        // error, expected an arrow, but did not receive one
    }

    advance(); // advance to expected type

    return parseType();
}

} // namespace sodium
