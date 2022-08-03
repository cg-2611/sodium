#ifndef SODIUM_NAC_ERROR_ERROR_MANAGER_H
#define SODIUM_NAC_ERROR_ERROR_MANAGER_H

#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "error.h"

namespace nac {

/**
 * A static class used to collect errors that are generated  during compilation.
 */
class ErrorManager {
public:
    /**
     * Creates and pushes an nac::Error of class E to the vector of errors.
     * @param args The arguments required to construct a new nac::Error of class E.
     */
    template<class E, typename... Args>
    static void addError(Args &&...args) {
        errors_.push_back(E(std::forward<Args>(args)...));
    }

    /**
     * @return True if the errors vector contains instances of nac::Errors.
     */
    static bool hasErrors() noexcept;

    /**
     * @return The total number of nac::Error objects in the errors vector.
     */
    static size_t errorCount() noexcept;

    /**
     * Constructs a string containing the messages for all the errors in the errors vector.
     * @return A formatted std::string containing all the error messages from the errors in the errors vector.
     */
    static std::string_view errorMessages();

private:
    // the errors that have been generated during compilation
    static std::vector<Error> errors_;
    static std::string errorMessages_;

    ErrorManager() = default;
    ~ErrorManager() = default;
};

} // namespace nac

#endif // SODIUM_NAC_ERROR_ERROR_MANAGER_H
