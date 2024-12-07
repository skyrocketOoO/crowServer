#include "crow_all.h"
#include <json.hpp>
#include <string>
#include <iostream>
#include <type_traits>
#include <utils/parser.hpp>
#include <utils/validator.hpp>
#include "rfl.hpp"

struct Request {
    std::string id;
    std::optional<std::string> name;

    struct Nested {
        std::string name;
        int value;
    } nested;
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

    // Test case 4: Missing nested object "nested"
    // std::string missingNested = R"({
    //     "id": "12345",
    //     "name": "Sample Name"
    // })";
    // auto [result4, err4] = parseJsonToStruct<Request>(missingNested);
    // if (err4 != "") {
    //     std::cout << "Missing nested object error: \n" << err4 << std::endl << std::endl;
    // }

    // Test case 5: Incorrect type in nested object
    // std::string incorrectNestedType = R"({
    //     "nested": "nested"
    // })";
    // auto [result5, err5] = parseJsonToStruct<Request>(incorrectNestedType);
    // if (err5 != "") {
    //     std::cout << "Incorrect nested type error: \n" << err5 << std::endl << std::endl;
    // }

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


    return 0;
}
