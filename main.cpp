#include "crow_all.h"
#include <json.hpp>
#include <string>
#include <iostream>
#include <type_traits>
#include "reflect-utils/parser/parser.hpp"
#include "reflect-utils/validator/validator.hpp"
#include "reflect-utils/validator/rule.hpp"
#include "rfl.hpp"

struct Request {
    std::string id;
    std::optional<std::string> name;

    struct Nested {
        std::string name;
        int value;

        auto validateMetas() {
            return std::tuple{
                Field<int>{"value", {Rule::Number::Min(0), Rule::Number::Max(20)}},
                Field<std::string>{"name", {Rule::Common::In<std::string>({"a", "b"})}},
            };
        }
    } nested;
    
    auto validateMetas() {
        return std::tuple{
            Field<std::string>{"name", {Rule::String::MaxLen(10)}},
        };
    }
};

int main() {
    // Test case 1: Missing required field "nested"
    std::string missingField = R"({
        "id": "12345",
        "name": "Sample Name"
    })";
    auto [result1, err1] = parseJsonToStruct<Request>(missingField);
    if (err1.size() != 0) {
        for (auto e : err1){
            std::visit([](const auto& err) {
                std::cout << static_cast<int>(err.type) << std::endl;
            }, e);
        }
        std::cout << std::endl;
    }

    // Test case 2: Incorrect type for "id"
    std::string incorrectType = R"({
        "id": 12345
    })";
    auto [result2, err2] = parseJsonToStruct<Request>(incorrectType);
    if (err2.size() != 0) {
        for (auto e : err2){
            std::visit([](const auto& err) {
                std::cout << static_cast<int>(err.type) << std::endl;
            }, e);
        }
        std::cout << std::endl;
    }

    // Test case 3: Unexpected field "desc"
    std::string unexpectedField = R"({
        "desc": "Unexpected field"
    })";
    auto [result3, err3] = parseJsonToStruct<Request>(unexpectedField);
    if (err3.size() != 0) {
        for (auto e : err3){
            std::visit([](const auto& err) {
                std::cout << static_cast<int>(err.type) << std::endl;
            }, e);
        }
        std::cout << std::endl;
    }

    // Test case 7: invalid JSON
    std::string invalidJson = R"({
        awefwafwafwafawafw
    })";
    auto [result7, err7] = parseJsonToStruct<Request>(invalidJson);
    if (err7.size() != 0) {
        for (auto e : err7){
            std::visit([](const auto& err) {
                std::cout << static_cast<int>(err.type) << std::endl;
            }, e);
        }
        std::cout << std::endl;
    }

    // Test case 6: Valid JSON
    std::string validJson = R"({
        "id": "12345",
        "name": "Sample Name",
        "nested": {
            "name": "Nested Name",
            "value": 42
        }
    })";
    auto [result6, err6] = parseJsonToStruct<Request>(validJson);
    if (err6.size() != 0) {
        for (auto e : err6){
            std::visit([](const auto& err) {
                std::cout << static_cast<int>(err.type) << std::endl;
            }, e);
        }
        std::cout << std::endl;
    }

    auto err = validate(result6);
    if (err != ""){
        std::cout << "Validation failed: " << err << std::endl;
    }

    return 0;
}
