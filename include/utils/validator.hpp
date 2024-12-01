#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

#include <string>
#include <vector>
#include "rfl.hpp"
#include "rfl/json.hpp"

struct FieldMeta {
    std::string name;
    std::vector<std::string> rules;
};

namespace Rule {
  std::string Required();

  namespace String{
    std::string MaxLength(int val);
  }

  namespace Number{
    std::string Min(int val);
  }
}

template <typename T>
struct is_user_defined : std::integral_constant<bool, !std::is_fundamental<T>::value && !std::is_enum<T>::value && !std::is_same<T, std::string>::value> {};

template <typename T, typename = void>
struct has_metadata : std::false_type {};

template <typename T>
struct has_metadata<T, std::void_t<decltype(std::declval<T>().metadata())>> : std::true_type {};


template <typename T>
std::string validate(T obj) {
  const auto view = rfl::to_view(obj);
  std::string err;
  std::array metadata = obj.metadata();

  view.apply([&err, &metadata](const auto& f) {
    if constexpr (
      std::is_class_v<std::decay_t<decltype(*f.value())>> && 
      is_user_defined<std::decay_t<decltype(*f.value())>>::value &&
      has_metadata<std::decay_t<decltype(*f.value())>>::value
      ) {
      err = validate(*f.value());
      return;
    }

    for (const FieldMeta& fieldMeta : metadata) {
      if (fieldMeta.name != f.name()) {
        continue;
      }
      auto value = rfl::json::write(*f.value());

      for (const std::string& rule : fieldMeta.rules) {
        // std::cout << rule << std::endl;
        if (rule == Rule::Required()) {
          if (value.empty()) {
              err = "Field '" + fieldMeta.name + "' is required";
              return;
          }
        } else if (rule.rfind("maxLength:", 0) == 0) {
          int maxLen = std::stoi(rule.substr(10));

          if (value.size() > maxLen) {
              err = "Field '" + fieldMeta.name + "' exceeds max length of " + std::to_string(maxLen);
              return; 
          }
        } else if (rule.rfind("min:", 0) == 0){
          int min = std::stoi(rule.substr(4));
          if (std::stoi(value) < min) {
              err = "Field '" + fieldMeta.name + "' small to  min " + std::to_string(min);
              return; 
          }
        }else {
          err = "Unknown validation rule for field '" + fieldMeta.name + "'";
          return; 
        }
      }
    }
  });

  return err;
}

#endif // VALIDATOR_HPP