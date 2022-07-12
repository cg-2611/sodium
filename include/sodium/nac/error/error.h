#ifndef SODIUM_NAC_ERROR_H
#define SODIUM_NAC_ERROR_H

#include <exception>
#include <string>

namespace sodium::nac {

class NACException : public std::exception {
protected:
    int code_;
    std::string message_;

public:
    NACException(int code, std::string message);
    virtual ~NACException() {};

    virtual const char *what() const noexcept override;
};

} // namespace sodium::nac

#endif // SODIUM_NAC_ERROR_H
