#include "sodium/nac/lexer/lexer.h"

#include <cctype>

#include "sodium/nac/error/error.h"

sodium::nac::Lexer::Lexer(const std::string &fileContents) {
    fileContents_ = fileContents;
    index_ = 0;
    character_ = fileContents_[index_];
}

std::shared_ptr<sodium::nac::Token> sodium::nac::Lexer::tokenize() {
    try {
        std::shared_ptr<sodium::nac::Token> token = getNextToken();
        std::shared_ptr<sodium::nac::Token> currentToken = token;

        advance(currentToken->getValue().size());

        while(character_ != '\0') {
            skipWhitespace();

            std::shared_ptr<sodium::nac::Token> nextToken = getNextToken();
            currentToken->setNext(nextToken);
            currentToken = currentToken->getNext();

            advance(currentToken->getValue().size());
        }

        return token;
    } catch (const sodium::nac::NACException &e) {
        // pass exception to the calling function
        throw e;
    }
}

std::shared_ptr<sodium::nac::Token> sodium::nac::Lexer::getNextToken() {
    if (validIdentifierFirstCharacter(character_)) {
        std::string identifier = readIdentifier();

        if (isKeyword(identifier)) {
            return std::make_shared<sodium::nac::Token>(Token(sodium::nac::TokenKind::TOKEN_KEYWORD, identifier));
        }

        if (isType(identifier)) {
            return std::make_shared<sodium::nac::Token>(Token(sodium::nac::TokenKind::TOKEN_TYPE, identifier));
        }

        return std::make_shared<sodium::nac::Token>(Token(sodium::nac::TokenKind::TOKEN_IDENTIFIER, identifier));
    }

    if (isdigit(character_)) {
        std::string numericLiteral = readNumericLiteral();
        return std::make_shared<sodium::nac::Token>(Token(sodium::nac::TokenKind::TOKEN_NUMERIC_LITERAL, numericLiteral));
    }

    switch (character_) {
        case '(': return std::make_shared<sodium::nac::Token>(Token(sodium::nac::TokenKind::TOKEN_LEFT_PAREN, "("));
        case ')': return std::make_shared<sodium::nac::Token>(Token(sodium::nac::TokenKind::TOKEN_RIGHT_PAREN, ")"));
        case '{': return std::make_shared<sodium::nac::Token>(Token(sodium::nac::TokenKind::TOKEN_LEFT_BRACE, "{"));
        case '}': return std::make_shared<sodium::nac::Token>(Token(sodium::nac::TokenKind::TOKEN_RIGHT_BRACE, "}"));
        case ':': return std::make_shared<sodium::nac::Token>(Token(sodium::nac::TokenKind::TOKEN_COLON, ":"));
        case ';': return std::make_shared<sodium::nac::Token>(Token(sodium::nac::TokenKind::TOKEN_SEMI_COLON, ";"));
        case '\0': return std::make_shared<sodium::nac::Token>(Token(sodium::nac::TokenKind::TOKEN_EOF, ""));
        default:
            std::string message = std::string("unexpected token \'\'");
            message.insert(message.size() - 1, 1, character_);
            throw sodium::nac::NACException(message);
    }

}

void sodium::nac::Lexer::advance(size_t offset) {
    index_ += offset;

    if (index_ >= fileContents_.size() || character_ == '\0') {
        index_ = fileContents_.size();
    }

    character_ = fileContents_[index_];
}

void sodium::nac::Lexer::skipWhitespace() {
    while (isspace(character_)) {
        advance(1);
    }
}

std::string sodium::nac::Lexer::readIdentifier() {
    std::string identifier = std::string(1, character_);
    size_t identifierIndex = index_ + 1;

    while (validIdentifierCharacter(fileContents_[identifierIndex])) {
        identifier += fileContents_[identifierIndex++];
    }

    return identifier;
}

std::string sodium::nac::Lexer::readNumericLiteral() {
    std::string numericLiteral = std::string(1, character_);
    size_t numericLiteralIndex = index_ + 1;

    while (isdigit(fileContents_[numericLiteralIndex])) {
        numericLiteral += fileContents_[numericLiteralIndex++];
    }

    return numericLiteral;
}

bool sodium::nac::Lexer::validIdentifierFirstCharacter(char c) {
    return isalpha(c) || c == '_' || c == '$';
}

bool sodium::nac::Lexer::validIdentifierCharacter(char c) {
    return validIdentifierFirstCharacter(c) || isdigit(c);
}

bool sodium::nac::Lexer::isKeyword(const std::string &identifier) {
    return sodium::nac::Token::KEYWORDS.find(identifier) != sodium::nac::Token::KEYWORDS.end();
}

bool sodium::nac::Lexer::isType(const std::string &identifier) {
    return sodium::nac::Token::TYPES.find(identifier) != sodium::nac::Token::TYPES.end();
}
