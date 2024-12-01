#include "validator.hpp"

namespace Rule {
    std::string Required() {
        return "required";
    }

    namespace String{
        std::string MaxLength(int val) {
            return "maxLength:" + std::to_string(val);
        }
    }

    namespace Number{
        std::string Min(int val){
            return "min:" + std::to_string(val);
        }
    }
}
