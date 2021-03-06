#ifndef NAC_ERROR_ERROR_H
#define NAC_ERROR_ERROR_H

#include <exception>
#include <string>

namespace nac {

/**
 * Used to handle errors from the Sodium compiler. Inherits std::exception.
*/
class Exception : public std::exception {
public:
    /**
     * Constructor for Exception.
     * @param message The message to be output when the what() method is called.
    */
    Exception(std::string message);

    /**
     * Destructor for Exception.
    */
    virtual ~Exception() {}

    /**
     * Inherited from std::exception.
    */
    virtual const char *what() const noexcept override;

protected:
    // the error message to be output when what() is called
    std::string message_;
};

} // namespace nac

#endif // NAC_ERROR_ERROR_H
