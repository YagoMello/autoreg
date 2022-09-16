#include <string>
#include "../../autoreg.hpp"

struct food {
    virtual std::string taste() const {
        return "undef.";
    }
};
