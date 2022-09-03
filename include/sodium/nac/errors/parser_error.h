#ifndef SODIUM_NAC_ERRORS_PARSER_ERROR_H
#define SODIUM_NAC_ERRORS_PARSER_ERROR_H

#include <string_view>

#include "sodium/nac/errors/error.h"

namespace sodium {

class Token;

/**
 * @brief A derived class of Error for errors generated in the parser.
 *
 */
class ParserError : public Error {
public:
    /**
     * @brief Construct a new Parser Error object.
     *
     * @param kind The kind of the error, must be one of ErrorKind.
     * @param token The token that has caused the error.
     * @param message The message that is displayed with the token.
     */
    ParserError(ErrorKind kind, Token *token, std::string_view message);
};

} // namespace sodium

#endif // SODIUM_NAC_ERRORS_PARSER_ERROR_H
