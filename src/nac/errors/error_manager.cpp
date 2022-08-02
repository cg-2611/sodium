#include "sodium/nac/errors/error_manager.h"

#include <string>
#include <string_view>
#include <vector>

#include "sodium/nac/errors/error.h"

namespace nac {

std::vector<Error> ErrorManager::errors_{};
std::string ErrorManager::errorMessages_("");

bool ErrorManager::hasErrors() noexcept {
    return !errors_.empty();
}

size_t ErrorManager::errorCount() noexcept {
    return errors_.size();
}

std::string_view ErrorManager::errorMessages() {
    for (const Error &error : errors_) {
        errorMessages_ += error.getMessage() + "\n[nac]: ";
    }

    errorMessages_ += "generated " + std::to_string(errorCount()) + " errors";

    return std::string_view(errorMessages_);
}

} // namespace nac
