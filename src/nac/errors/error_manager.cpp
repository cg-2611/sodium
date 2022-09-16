#include "sodium/nac/errors/error_manager.h"

#include <string>
#include <string_view>
#include <vector>

#include "sodium/nac/errors/error.h"

namespace sodium {

std::vector<Error> ErrorManager::errors_{};
std::string ErrorManager::error_messages_{};

bool ErrorManager::has_errors() noexcept {
    return !errors_.empty();
}

size_t ErrorManager::error_count() noexcept {
    return errors_.size();
}

std::string_view ErrorManager::error_messages() {
    for (const Error &error : errors_) {
        error_messages_ += error.message() + "\n[nac]: ";
    }

    error_messages_ += "generated " + std::to_string(error_count()) + " errors";

    return std::string_view(error_messages_);
}

} // namespace sodium
