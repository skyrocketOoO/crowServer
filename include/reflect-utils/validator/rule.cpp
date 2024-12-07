#include "rule.hpp"
#include <numeric>

namespace Rule {
    namespace Common {

        template <typename T>
        std::function<std::string(std::string, T)> And(std::vector<std::function<std::string(std::string, T)>> validateFuncs){
            return [validateFuncs](std::string fName, T fVal) {
                std::string errorMsg;
                for (auto validateFunc : validateFuncs) {
                    std::string error = validateFunc(fName, fVal);
                    if (!error.empty()) {
                        errorMsg += error + "\n";
                    }
                }
                return errorMsg;
            };
        }

        template <typename T>
        std::function<std::string(std::string, T)> Eq(T value){
            return [value](std::string fName, T fVal) {
                if (fVal != value){
                    return "Field '" + fName + "' value is not equal to " + std::to_string(value);
                };
                return std::string{};
            };
        }

        template <typename T>
        std::function<std::string(std::string, T)> Ne(T value){
            return [value](std::string fName, T fVal) {
                if (fVal == value){
                    return "Field '" + fName + "' value is equal to " + std::to_string(value);
                };
                return std::string{};
            };
        }
    }

    namespace String{
        using ret = std::function<std::string(std::string, std::string)>;
        ret MaxLen(int threshold) {
            return [threshold](std::string fName, std::string fVal) {
                if (fVal.size() > threshold){
                    return "Field '" + fName + "' size is longer than " + std::to_string(threshold);
                };
                return std::string{};
            };
        }
        ret RegExp(const std::string pattern){
            return [pattern](std::string fName, std::string fVal) {
                
                return std::string{};
            };
        }
    }

    namespace Number{
        using ret = std::function<std::string(std::string, int)>;
        ret Min(int threshold) {
            return [threshold](std::string fName, int fVal) {
                if (fVal < threshold){
                    return "Field '" + fName + "' is smaller than " + std::to_string(threshold);
                };
                return std::string{};
            };
        }
        ret Max(int threshold) {
            return [threshold](std::string fName, int fVal) {
                if (fVal > threshold){
                    return "Field '" + fName + "' is bigger than " + std::to_string(threshold);
                };
                return std::string{};
            };
        }
    }
}
