#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

#include <string>
#include <vector>
#include "rfl.hpp"
#include "rfl/json.hpp"
#include <any>
#include <array>
#include <iostream>
#include <typeinfo>
#include <stdexcept>

enum class RuleType {
    MaxLen,
    RegExp,
    Min,
    Max,
    In,
    Eq,
    NotEq,
};

struct FieldMeta {
  std::string name;
  std::vector<RuleMeta> ruleMetas;
};

struct RuleMeta {
  RuleType type;
  std::any value;
};

namespace Rule {
  RuleMeta In(const std::vector<std::any>);
  namespace String{
    RuleMeta MaxLen(int val);
    RuleMeta RegExp(const std::string);
  }

  namespace Number{
    RuleMeta Min(int val);
    RuleMeta Max(int val);
  }
}

template <typename T>
struct is_user_defined : std::integral_constant<bool, !std::is_fundamental<T>::value && !std::is_enum<T>::value && !std::is_same<T, std::string>::value> {};

template <typename T, typename = void>
struct has_metadata : std::false_type {};

template <typename T>
struct has_metadata<T, std::void_t<decltype(std::declval<T>().metadata())>> : std::true_type {};

template <typename T>
struct is_optional : std::false_type {};
template <typename T>
struct is_optional<std::optional<T>> : std::true_type {};
// Helper variable for convenience (C++17)
template <typename T>
inline constexpr bool is_optional_v = is_optional<T>::value;

template <typename T>
std::string validate(T obj) {
    const auto view = rfl::to_view(obj);
    std::string err;
  
    view.apply([&err, &obj](const auto& field) {
      if (err != ""){
        return;
      }
      
      // check and unwrapped std::optional
      auto& value = *field.value();
      std::any valueAny;
      using FieldType = std::decay_t<decltype(*field.value())>;
      if constexpr (is_optional_v<FieldType>) { 
        if (!value.has_value()) {
            return;
        }
        valueAny = value.value();
      }else{
        valueAny = value;
      }

      // check nested
      if constexpr (
        std::is_class_v<FieldType> && 
        is_user_defined<FieldType>::value &&
        has_metadata<FieldType>::value
      ) {
        err = validate(value);
        return; 
      }

      using objType = std::decay_t<decltype(obj)>;
      if constexpr (has_metadata<objType>::value) {
        auto metadatas = obj.metadata();
        for (const FieldMeta& metadata : metadatas) {
          if (metadata.name != field.name()) {
              continue;
          }

          for (const std::string& rule : metadata.rules) {
            try {
                if (rule.rfind("maxLen:", 0) == 0) {
                    int maxLen = std::stoi(rule.substr(7));
                    if (valueAny.type() == typeid(std::string)) {
                      if (std::any_cast<std::string>(valueAny).size() > maxLen) {
                        err = "Field '" + metadata.name + "' exceeds max length of " + std::to_string(maxLen);
                        return;
                      }
                    } else {
                        err = "Field '" + metadata.name + "' does not support size()";
                        return;
                    }
                } else if (rule.rfind("min:", 0) == 0) {
                    int min = std::stoi(rule.substr(4));
                    if (valueAny.type() == typeid(int)) {
                        if (std::any_cast<int>(valueAny) < min) {
                            err = "Field '" + metadata.name + "' is smaller than min " + std::to_string(min);
                            return;
                        }
                    } else {
                        err = "Field '" + metadata.name + "' does not support comparison";
                        return;
                    }
                } else {
                    err = "Unknown validation rule for field '" + metadata.name + "'";
                    return;
                }
            } catch (const std::exception& e) {
                err = e.what();
                return;
            }
          }
        }
      }
    });

    return err;
}
#endif // VALIDATOR_HPP