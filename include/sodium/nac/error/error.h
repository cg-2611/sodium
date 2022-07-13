#ifndef SODIUM_NAC_ERROR_ERROR_H
#define SODIUM_NAC_ERROR_ERROR_H

#include <exception>
#include <string>

namespace sodium::nac {

class NACException : public std::exception {
public:
    NACException(std::string message);
    virtual ~NACException() {}

    virtual const char *what() const noexcept override;

protected:
    std::string message_;
};

} // namespace sodium::nac

#endif // SODIUM_NAC_ERROR_ERROR_H
