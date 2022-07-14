#ifndef SODIUM_NAC_ERROR_ERROR_H
#define SODIUM_NAC_ERROR_ERROR_H

#include <exception>
#include <string>

namespace sodium::nac {

/**
 * This class inherits std::exception and is used to handle sodium compiler errors.
*/
class NACException : public std::exception {
public:
    /**
     * Constructor for NACException. The string value of errno is appended to the message argument
     * when this is called.
     * @param message the message to be output when the what() method is called.
    */
    NACException(std::string message);

    /**
     * Destructor for NACException.
    */
    virtual ~NACException() {}

    /**
     * Inherited from std::exception.
    */
    virtual const char *what() const noexcept override;

protected:
    std::string message_;
};

} // namespace sodium::nac

#endif // SODIUM_NAC_ERROR_ERROR_H
