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

template <typename T>
struct Field {
    std::string name;
    std::vector<std::function<std::string(std::string,T)>> validateFuncs;

    T castAny(const std::any& value) const {
        try {
            return std::any_cast<T>(value);
        } catch (const std::bad_any_cast&) {
            throw std::runtime_error("Type mismatch: failed to cast std::any to the required type.");
        }
    }

    // TODO: Add static check to avoid the T is not fit the field type
};

template <typename T>
concept isUserDefined = 
    !std::is_fundamental_v<T> && 
    !std::is_enum_v<T> && 
    !std::is_same_v<T, std::string>;

template <typename T>
concept hasValidateMeta = requires(T t) {
    { t.validateMetas() };
};

// template <typename T>
// concept isOptional = requires(T t){
//   { std::optional<T>{t} } -> std::same_as<std::optional<T>>;
// }

template <typename Tuple, typename Func, std::size_t... Indices>
void forEachInTuple(const Tuple& tuple, Func&& func, std::index_sequence<Indices...>) {
    (func(std::get<Indices>(tuple)), ...);
}

template <typename Tuple, typename Func>
void forEachInTuple(const Tuple& tuple, Func&& func) {
    forEachInTuple(tuple, std::forward<Func>(func), std::make_index_sequence<std::tuple_size_v<Tuple>>{});
}

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
      if constexpr (std::__is_optional_v<FieldType>) { 
        if (!value.has_value()) {
            return;
        }
        valueAny = value.value();
      }else{
        valueAny = value;
      }

      if constexpr (
        std::is_class_v<FieldType> && 
        isUserDefined<FieldType> &&
        hasValidateMeta<FieldType>
      ) {
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