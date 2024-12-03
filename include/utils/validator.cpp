#include "validator.hpp"

namespace Rule {

    namespace String{
        RuleMeta MaxLen(int val) {
            return RuleMeta{
                type: RuleType::MaxLen,
                value: val,
            };
        }
        RuleMeta RegExp(const std::string s){
            return "regexp:" + s;
        }
        RuleMeta In(const std::vector<std::string> v){
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
