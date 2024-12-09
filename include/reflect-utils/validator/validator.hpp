#pragma once

#include <string>
#include <vector>
#include "rfl.hpp"
#include "rfl/json.hpp"
#include <any>
#include <array>
#include <iostream>
#include <typeinfo>
#include <stdexcept>
#include "rule.hpp"
#include <concepts>
#include "tuple.hpp"
#include "Field.hpp"

template <typename T>
concept isUserDefined = 
    !std::is_fundamental_v<T> && 
    !std::is_enum_v<T> && 
    !std::is_same_v<T, std::string>;

template <typename T>
concept hasValidateMeta = requires(T t) {
    { t.validateMetas() };
};

template <typename T>
concept IsNestedStruct = std::is_class_v<T> && isUserDefined<T> && hasValidateMeta<T>;     

template <typename T>
std::string validate(T obj) {
    const auto view = rfl::to_view(obj);
    std::string err;

    view.apply([&err, &obj](const auto& field) {
      // check and unwrapped std::optional
      auto& value = *field.value();
      std::any valueAny = value;
      using FieldType = std::decay_t<decltype(*field.value())>;
      if constexpr (std::__is_optional_v<FieldType>) { 
        if (!value.has_value()) {
            return;
        }
        valueAny = value.value();
      }

      if constexpr (IsNestedStruct<FieldType>) {
        err = validate(value);
        return; 
      }

      if constexpr (!hasValidateMeta<T>) {
        return;
      }

      auto validateMetas = obj.validateMetas();
      forEachInTuple(validateMetas, [&err, &field, &valueAny](const auto& metadata) {
        // std::cout << field.name() << " : " << metadata.name << std::endl;
        if (metadata.name != field.name()) {
            return;
        }

        for (const auto validateFunc : metadata.validateFuncs) {
          try {
              err = validateFunc(metadata.name, metadata.castAny(valueAny));
              if (err != ""){
                return;
              }
          } catch (const std::exception& e) {
              err = e.what();
              return;
          }
        }
      });
    });

    return err;
}