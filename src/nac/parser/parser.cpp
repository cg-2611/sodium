#include "sodium/nac/parser/parser.h"

#include <memory>
#include <string>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <vector>

#include "sodium/nac/ast/ast.h"
#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/identifier.h"
#include "sodium/nac/ast/source_file.h"
#include "sodium/nac/ast/type.h"
#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/errors/parser_error.h"
#include "sodium/nac/lexer/token.h"

namespace sodium {

const std::unordered_set<TokenKind> Parser::DECL_SYNCHRONIZING_TOKENS{TokenKind::RIGHT_BRACE};
const std::unordered_set<TokenKind> Parser::STMT_SYNCHRONIZING_TOKENS{TokenKind::LEFT_BRACE, TokenKind::RIGHT_BRACE,
                                                                      TokenKind::SEMICOLON};

Parser::Parser(Token *token) : token_(token) {}

std::unique_ptr<AST> Parser::parse() {
    // a source file is the root of the AST
    std::unique_ptr<ASTNode> root(parseSourceFile());
    return std::make_unique<AST>(std::move(root));
}

std::unique_ptr<SourceFile> Parser::parseSourceFile() {
    std::vector<std::unique_ptr<Decl>> decls{};

    // parse declarations until we reach an EOF token
    while (token_->kind() != TokenKind::EOF_TOKEN) {
        std::unique_ptr<Decl> decl(parseDecl());
        if (!decl) {
            synchronize(DECL_SYNCHRONIZING_TOKENS);
        }

        decls.push_back(std::move(decl));
    }

    return std::make_unique<SourceFile>(std::move(decls));
}

std::unique_ptr<Identifier> Parser::parseIdentifier() {
    if (!match(TokenKind::IDENTIFIER)) {
        return nullptr;
    }

    return std::make_unique<Identifier>(token_->value());
}

std::unique_ptr<Type> Parser::parseType() {
    if (!match(TokenKind::TYPE)) {
        return nullptr;
    }

    return std::make_unique<Type>(token_->value());
}

void Parser::advance() noexcept {
    if (token_->kind() != TokenKind::EOF_TOKEN) {
        token_ = token_->next();
    }
}

bool Parser::match(TokenKind expectedTokenKind) const {
    return token_->kind() == expectedTokenKind;
}

bool Parser::expect(TokenKind expectedTokenKind, std::string_view message) const {
    bool tokenMatch = match(expectedTokenKind);

    if (!tokenMatch) {
        errorExpected(message);
    }

    return tokenMatch;
}

void Parser::errorExpected(std::string_view message) const {
    std::string errorMessage("expected ");
    errorMessage += message;
    ErrorManager::addError<ParserError>(ErrorKind::SYNTAX_ERROR, token_, errorMessage);
}

void Parser::synchronize(const std::unordered_set<TokenKind> &synchronizingTokens) {
    while (token_->kind() != TokenKind::EOF_TOKEN) {
        if (synchronizingTokens.contains(token_->kind())) {
            break;
        }

        advance();
    }
}

} // namespace sodium
