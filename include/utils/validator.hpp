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
}

template <typename T>
std::string validate(T obj) {
  const auto view = rfl::to_view(obj);
  std::string err;

  view.apply([&err, &obj](const auto& f) { // Capture err and obj by reference
      for (const FieldMeta& fieldMeta : obj.metadata()) {
          if (fieldMeta.name == f.name()) {
              auto value = rfl::json::write(*f.value());

              for (const auto& rule : fieldMeta.rules) {
                  if (rule == Rule::Required()) {
                      if (value.empty()) {
                          err = "Field '" + fieldMeta.name + "' is required";
                          return; // Break out of the lambda
                      }
                  } else if (rule.rfind("maxLength:", 0) == 0) {
                      int maxLen = std::stoi(rule.substr(10));

                      if (value.size() > maxLen) {
                          err = "Field '" + fieldMeta.name + "' exceeds max length of " + std::to_string(maxLen);
                          return; // Break out of the lambda
                      }
                  } else {
                      err = "Unknown validation rule for field '" + fieldMeta.name + "'";
                      return; // Break out of the lambda
                  }
              }
          }
      }
  });


    // for (const FieldMeta& fieldMeta : obj.metadata()) {
    //     for (const auto& rule : fieldMeta.rules) {
    //       if (rule == Rule::Required()){

    //       }else if (rule.rfind("maxLength:", 0) == 0){
    //         int maxLen = std::stoi(rule.substr(10));
    //         auto field = *rfl::get<fieldMeta.name>(view);
            
    //         if (field != nullptr){
    //           if (field.length() > maxLen) {
    //             return "Field '" + fieldMeta.name + "' exceeds max length of " + std::to_string(maxLen);
    //           }
    //         }else{
    //           return "Field '" + fieldMeta.name + "' is required";
    //         }
    //       }
    //     }
    // }

    // Check nested structs
    // if constexpr (requires { obj.nested; }) {
    //     auto nestedError = validate(obj.nested);
    //     if (!nestedError.empty()) {
    //         return nestedError;
    //     }
    // }
    return err;
}

#endif // VALIDATOR_HPP