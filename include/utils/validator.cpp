#include "validator.hpp"

namespace Rule {

    namespace String{
        std::string MaxLen(int val) {
            return "maxLen:" + std::to_string(val);
        }
        std::string RegExp(const std::string s){
            return "regexp:" + s;
        }
        std::string In(const std::vector<std::string> v){
            std::string result = "in:[";
            for(const auto& s : v){
                result += s + ", ";
            }
            result.pop_back();
            result.pop_back();
            result += "]";
            return result;
        }
    }

    namespace Number{
        std::string Min(int val){
            return "min:" + std::to_string(val);
        }
        std::string Max(int val){
            return "max:" + std::to_string(val);
        }
    }
}
