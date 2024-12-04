#include "validator.hpp"

namespace Rule {

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
        // std::string MaxLen(int val) {
        //     return "maxLen:" + std::to_string(val);
        // }
        // std::string RegExp(const std::string s){
        //     return "regexp:" + s;
        // }
        // std::string In(const std::vector<std::string> v){
        //     std::string result = "in:[";
        //     for(const auto& s : v){
        //         result += s + ", ";
        //     }
        //     result.pop_back();
        //     result.pop_back();
        //     result += "]";
        //     return result;
        // }
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
