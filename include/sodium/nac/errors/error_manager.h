#ifndef SODIUM_NAC_ERROR_ERROR_REPORTER_H
#define SODIUM_NAC_ERROR_ERROR_REPORTER_H

#include <string>
#include <string_view>
#include <vector>

#include "error.h"

namespace nac {

class ErrorManager {
public:
    template<class E, class... Args>
    static void addError(Args &&...args) {
        errors_.push_back(E(std::forward<Args>(args)...));
    }

    static bool hasErrors() noexcept;
    static size_t errorCount() noexcept;

    static std::string_view errorMessages();

private:
    static std::vector<Error> errors_;
    static std::string errorMessages_;

    ErrorManager() = default;
    ~ErrorManager() = default;
};

} // namespace nac

#endif // SODIUM_NAC_ERROR_ERROR_REPORTER_H
