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
            return RuleMeta{
                type: RuleType::RegExp,
                value: s,
            };
        }
    }

    namespace Number{
        RuleMeta Min(int val){
            return RuleMeta{
                type: RuleType::Min,
                value: val,
            };
        }
        RuleMeta Max(int val){
            return RuleMeta{
                type: RuleType::Max,
                value: val,
            };
        }
    }
}
