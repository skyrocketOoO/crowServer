// Validator.cpp
#include "validator.hpp"

namespace Rule {
    std::string Required() {
        return "required";
    }

    std::string MaxLength(int val) {
        return "maxLength:" + std::to_string(val);
    }
}
