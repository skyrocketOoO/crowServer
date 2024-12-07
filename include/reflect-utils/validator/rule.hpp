#ifndef RULE_HPP
#define RULE_HPP

#include <string>
#include <functional>
#include <numeric>
#include <vector>  
#include <sstream> // For std::ostringstream
#include <type_traits> 

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
                    oss << val; // Works for std::string and types with operator<<
                    return oss.str();
                }
            };

            return "Field '" + fName + "' must be one of: " +
                std::accumulate(values.begin(), values.end(), std::string{},
                                [&toString](std::string a, const T& v) {
                                    if (!a.empty()) {
                                        return a + ", " + toString(v);
                                    }
                                    return toString(v);
                                });
        };
    }


    template <typename T>
    std::function<std::string(std::string, T)> And(std::vector<std::function<std::string(std::string, T)>> validateFuncs);

    template <typename T>
    std::function<std::string(std::string, T)> Or(std::vector<std::function<std::string(std::string, T)>> validateFuncs){
        return [validateFuncs](std::string fName, T fVal) {
            std::string errorMsg;
            bool oneSuccess = false;
            for (auto validateFunc : validateFuncs) {
                std::string error = validateFunc(fName, fVal);
                if (!error.empty()) {
                    if (!errorMsg.empty()){
                      errorMsg += "OR\n";
                    }
                    errorMsg += error + "\n";
                }else{
                    oneSuccess = true;
                    break;
                }
            }
            if (oneSuccess){
                return std::string{};
            }
            return errorMsg;
        };
    }

    template <typename T>
    std::function<std::string(std::string, T)> Eq(T value);
  
    template <typename T>
    std::function<std::string(std::string, T)> Ne(T value);

    template <typename T>
    std::function<std::string(std::string, T)> NotWritable(){
        return [](std::string fName, T fVal) {
            return "Field '" + fName + "' is not writable";
        };
    }
  }
  namespace String{
    using ret = std::function<std::string(std::string, std::string)>;
    ret MaxLen(int threshold);
    ret RegExp(std::string pattern);
  }

  namespace Number{
    using ret = std::function<std::string(std::string, int)>;
    ret Min(int threshold);
    ret Max(int threshold);
    inline ret Gt(int value){
      return [value](std::string fName, int fVal) {
          if (fVal <= value){
              return "Field '" + fName + "' is smaller and equal than " + std::to_string(value);
          };
          return std::string{};
      };
    }

    inline ret Lt(int value){
      return [value](std::string fName, int fVal) {
          if (fVal >= value){
              return "Field '" + fName + "' is bigger and equal than " + std::to_string(value);
          };
          return std::string{};
      };
    }
  }
}

#endif // RULE_HPP