#ifndef SODIUM_NAC_ERROR_ERROR_H
#define SODIUM_NAC_ERROR_ERROR_H

#include <string>

namespace nac {

enum class ErrorKind {
    UNRECOGNISED_TOKEN
};

class Error {
public:
    Error(ErrorKind kind, size_t line, size_t column);
    virtual ~Error() = default;

    const std::string &getMessage() const;

protected:
    std::string message_;
};

} // namespace nac

#endif // SODIUM_NAC_ERROR_ERROR_H
