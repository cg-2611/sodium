#include "sodium/nac/parser/parser.h"

#include <memory>
#include <utility>
#include <unordered_set>
#include <vector>

#include "sodium/nac/ast/decl.h"
#include "sodium/nac/ast/type.h"
#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/errors/parser_error.h"
#include "sodium/nac/lexer/token.h"

namespace sodium {

static const std::unordered_set<TokenKind> DECL_SYNCHRONIZING_TOKENS = {TokenKind::RIGHT_BRACE};

std::unique_ptr<Decl> Parser::parseDecl() {
    switch (token_->kind()) {
        case TokenKind::KEYWORD:
            if (token_->value() == "func") {
                std::unique_ptr<FuncDecl> funcDecl(parseFuncDecl());
                if (funcDecl == nullptr) {
                    // expected function declaration
                    ErrorManager::addError<ParserError>(ErrorKind::SYNTAX_ERROR, token_,
                                                        "expected function declaration");
                    synchronize(DECL_SYNCHRONIZING_TOKENS);
                    // return nullptr;
                }
                return funcDecl;
            }
            [[fallthrough]];
        default:
            // expected declaration
            ErrorManager::addError<ParserError>(ErrorKind::SYNTAX_ERROR, token_, "expected declaration");
            synchronize(DECL_SYNCHRONIZING_TOKENS);
            return nullptr;
    }
}

std::unique_ptr<FuncDecl> Parser::parseFuncDecl() {
    std::unique_ptr<FunctionSignature> functionSignature(parseFunctionSignature());

    if (functionSignature == nullptr) {
        // expected function signature
        ErrorManager::addError<ParserError>(ErrorKind::SYNTAX_ERROR, token_, "expected function signature");
        return nullptr;
    }

    advance(); // advance to expected block

    std::unique_ptr<Block> functionBody(parseBlock());

    if (functionBody == nullptr) {
        // expected function body
        ErrorManager::addError<ParserError>(ErrorKind::SYNTAX_ERROR, token_, "expected function body");
        return nullptr;
    }

    return std::make_unique<FuncDecl>(std::move(functionSignature), std::move(functionBody));
}

std::unique_ptr<FunctionSignature> Parser::parseFunctionSignature() {
    advance(); // advance to expected identifier
    std::unique_ptr<Identifier> functionName(parseIdentifier());

    if (functionName == nullptr) {
        // expected function name
        ErrorManager::addError<ParserError>(ErrorKind::SYNTAX_ERROR, token_, "expected function name");
        return nullptr;
    }

    advance(); // advance to expected parameter list
    std::unique_ptr<ParameterList> functionParameters(parseParameterList());

    if (functionParameters == nullptr) {
        // expected function parameter list
        ErrorManager::addError<ParserError>(ErrorKind::SYNTAX_ERROR, token_, "expected function parameter list");
        return nullptr;
    }

    advance(); // advance to expected return type
    std::unique_ptr<Type> functionReturnType(parseReturnType());

    if (functionReturnType == nullptr) {
        // expected function return type
        ErrorManager::addError<ParserError>(ErrorKind::SYNTAX_ERROR, token_, "expected function return type");
        return nullptr;
    }

    return std::make_unique<FunctionSignature>(std::move(functionName), std::move(functionParameters),
                                               std::move(functionReturnType));
}

std::unique_ptr<ParameterList> Parser::parseParameterList() {
    if (token_->kind() != TokenKind::LEFT_PAREN) {
        // expected (
        ErrorManager::addError<ParserError>(ErrorKind::SYNTAX_ERROR, token_, "expected ( to begin parameter list");
        return nullptr;
    }

    advance(); // advance to the ) token

    if (token_->kind() != TokenKind::RIGHT_PAREN) {
        // expected ), bracket pair never closed
        ErrorManager::addError<ParserError>(ErrorKind::SYNTAX_ERROR, token_,
                                            "expected ) to end parameter list, pair never closed");
        return nullptr;
    }

    return std::make_unique<ParameterList>();
}

std::unique_ptr<Type> Parser::parseReturnType() {
    if (token_->kind() != TokenKind::ARROW) {
        // expected ->
        ErrorManager::addError<ParserError>(ErrorKind::SYNTAX_ERROR, token_, "expected ->");
        return nullptr;
    }

    advance(); // advance to expected type

    std::unique_ptr<Type> returnType(parseType());

    if (returnType == nullptr) {
        // expected type after ->
        ErrorManager::addError<ParserError>(ErrorKind::SYNTAX_ERROR, token_, "expected type after ->");
    }

    return returnType;
}

} // namespace sodium
