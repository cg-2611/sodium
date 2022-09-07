#ifndef SODIUM_NAC_ERROR_ERROR_MANAGER_H
#define SODIUM_NAC_ERROR_ERROR_MANAGER_H

#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "error.h"

namespace sodium {

/**
 * @brief A static class used to collect errors that are generated  during compilation.
 *
 */
class ErrorManager {
public:
    /**
     * @brief Creates and pushes an Error to the vector of errors.
     *
     * @tparam E The error type being pushed to the vector of errors.
     * @tparam Args Variadic template arguments that are forwarded to the constructor of E.
     * @param args The arguments required to construct a new sodium::Error of class E.
     */
    template<class E, typename... Args>
    static void add_error(Args &&...args) {
        errors_.push_back(E(std::forward<Args>(args)...));
    }

    /**
     * @brief Check if errors have been added to the vector of errors.
     *
     * @return true if there are errors in the vector,
     * @return false otherwise.
     */
    static bool has_errors() noexcept;

    /**
     * @brief Get the number of errors that are in the vector of errors.
     *
     * @return size_t The number of vectors in the vector of errors.
     */
    static size_t error_count() noexcept;

    /**
     * @brief Constructs an std::string_view containing the messages for all the errors in the errors vector.
     *
     * @return std::string_view that is a formatted string containing all the error messages from the errors in the
     *         errors vector.
     */
    static std::string_view error_messages();

private:
    // the errors that have been generated during compilation
    static std::vector<Error> errors_;
    static std::string error_messages_;

    // private constructor and destructor for static class
    ErrorManager() = default;
    ~ErrorManager() = default;
};

} // namespace sodium

#endif // SODIUM_NAC_ERROR_ERROR_MANAGER_H
