#include "sodium/nac/errors/error_manager.h"

#include <string>
#include <string_view>
#include <vector>

#include "sodium/nac/errors/error.h"
#include "sodium/nac/util/string_formatter.h"

namespace sodium {

std::vector<Error> ErrorManager::errors_{};
std::string ErrorManager::errorMessages_{};

bool ErrorManager::hasErrors() noexcept {
    return !errors_.empty();
}

size_t ErrorManager::errorCount() noexcept {
    return errors_.size();
}

std::string_view ErrorManager::errorMessages() {
    for (const Error &error : errors_) {
        errorMessages_ += error.message() + "\n[nac]: ";
    }

    errorMessages_ += StringFormatter::formatString("generated %lu errors", errorCount());

    return std::string_view(errorMessages_);
}

} // namespace sodium
