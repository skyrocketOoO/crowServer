#ifndef RULE_HPP
#define RULE_HPP

#include <string>
#include <functional>
#include <numeric>
#include <vector>  
#include <sstream>
#include <type_traits> 
#include "error.hpp"


namespace Rule {
  namespace Common {
    template <typename T>
    std::function<std::string(std::string, T)> In(std::vector<T> values) {
        return [values](std::string fName, T fVal) {
            for (const auto& v : values) {
                if (v == fVal) {
                    return std::string{}; 
                }
            }

            auto toString = [](const auto& val) -> std::string {
                if constexpr (std::is_arithmetic_v<std::decay_t<decltype(val)>>) {
                    return std::to_string(val);
                } else {
                    std::ostringstream oss;
                    oss << val;
                    return oss.str();
                }
            };

            return rfl::json::write(Validator::Error<T>{
                Validator::ErrorType::In,
                fName,
                fVal,
                values
            });
        };
    }


    template <typename T>
    std::function<std::string(std::string, T)> And(std::vector<std::function<std::string(std::string, T)>> validateFuncs){
        return [validateFuncs](std::string fName, T fVal) {
            std::vector<std::string> errs;
            for (auto validateFunc : validateFuncs) {
                std::string err = validateFunc(fName, fVal);
                if (!err.empty()) {
                    errs.push_back(err);
                }
            }
            if (errs.size() == 0){
                return std::string{}; 
            }

            std::vector<nlohmann::json> deserializedErrs;
            for (const auto& errStr : errs) {
                deserializedErrs.push_back(nlohmann::json::parse(errStr));
            }

            nlohmann::json errorJson = {
                {"type", "And"},
                {"errors", deserializedErrs}
            };
            return errorJson.dump();
        };
    }
    
    template <typename T>
    std::function<std::string(std::string, T)> Or(std::vector<std::function<std::string(std::string, T)>> validateFuncs){
        return [validateFuncs](std::string fName, T fVal) {
            std::vector<std::string> errs;
            for (auto validateFunc : validateFuncs) {
                std::string err = validateFunc(fName, fVal);
                if (!err.empty()) {
                    errs.push_back(err);
                }
            }
            if (errs.size() == 0){
                return std::string{}; 
            }

            std::vector<nlohmann::json> deserializedErrs;
            for (const auto& errStr : errs) {
                deserializedErrs.push_back(nlohmann::json::parse(errStr));
            }

            nlohmann::json errorJson = {
                {"type", "Or"},
                {"errors", deserializedErrs}
            };
            return errorJson.dump();
        };
    }

    template <typename T>
    std::function<std::string(std::string, T)> Eq(T value){
        return [value](std::string fName, T fVal) {
            if (fVal != value){
                return rfl::json::write(Validator::Error<int>{
                    Validator::ErrorType::Eq,
                    fName,
                    fVal,
                    value,
                });
            };
            return std::string{};
        };
    }

    template <typename T>
    std::function<std::string(std::string, T)> Ne(T value){
        return [value](std::string fName, T fVal) {
            if (fVal == value){
                return rfl::json::write(Validator::Error<int>{
                    Validator::ErrorType::Ne,
                    fName,
                    fVal,
                    value,
                });
            };
            return std::string{};
        };
    }

    template <typename T>
    std::function<std::string(std::string, T)> NotWritable(){
        return [](std::string fName, T fVal) {
            struct Error {
                Validator::ErrorType type;               
                std::string fieldName;
            };
            return rfl::json::write(Error{
                Validator::ErrorType::NotWritable,
                fName,
            });
        };
    }
  }
  namespace String{
    using ret = std::function<std::string(std::string, std::string)>;
    ret MaxLen(int threshold) {
        return [threshold](std::string fName, std::string fVal) {
            if (fVal.size() > threshold){
                struct Error {
                    Validator::ErrorType type;               
                    std::string fieldName;
                    std::string value;                       
                    int compareVal;
                };
                return rfl::json::write(Error{
                    Validator::ErrorType::NotWritable,
                    fName,
                    fVal,
                    threshold,
                });
            };
            return std::string{};
        };
    }
    // ret RegExp(const std::string pattern){
    //     return [pattern](std::string fName, std::string fVal) {
            
    //         return std::string{};
    //     };
    // }
  }

  namespace Number{
    using ret = std::function<std::string(std::string, int)>;
    ret Min(int threshold) {
        return [threshold](std::string fName, int fVal) {
            if (fVal < threshold){
                return rfl::json::write(Validator::Error<int>{
                    Validator::ErrorType::Min,
                    fName,
                    fVal,
                    threshold,
                });
            }
            return std::string{};
        };
    }
    ret Max(int threshold) {
        return [threshold](std::string fName, int fVal) {
            if (fVal > threshold){
                return rfl::json::write(Validator::Error<int>{
                    Validator::ErrorType::Max,
                    fName,
                    fVal,
                    threshold,
                });
            };
            return std::string{};
        };
    }
    inline ret Gt(int value){
      return [value](std::string fName, int fVal) {
          if (fVal <= value){
              return rfl::json::write(Validator::Error<int>{
                    Validator::ErrorType::Gt,
                    fName,
                    fVal,
                    value,
                });
          } 
          return std::string{};
      };
    }

    inline ret Lt(int value){
      return [value](std::string fName, int fVal) {
          if (fVal >= value){
              return rfl::json::write(Validator::Error<int>{
                    Validator::ErrorType::Lt,
                    fName,
                    fVal,
                    value,
                });
          }
          return std::string{};
      };
    }
  }
}

#endif // RULE_HPP