#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <iostream>
#include "reflect-utils/parser/parser.hpp"
#include "reflect-utils/validator/validator.hpp"
#include "reflect-utils/validator/rule.hpp"

struct Request {
    std::string id;
    std::optional<std::string> name;

    struct Nested {
        std::string name;
        int value;

        auto validateMetas() {
            return std::tuple{
                Field<int>{"value", {
                    Rule::Common::Or<int>({Rule::Number::Gt(10), Rule::Number::Lt(0)})
                }},
                Field<std::string>{"name", {Rule::Common::In<std::string>({"a", "b"})}},
            };
        }
    } nested;
    
    auto validateMetas() {
        return std::tuple{
            Field<std::string>{"name", {Rule::Common::NotWritable<std::string>()}},
            Field<std::string>{"id", {Rule::String::MaxLen(10)}}
        };
    }
};


TEST(JsonParsingTests, MissingRequiredField) {
    std::string missingField = R"({
        "id": "12345",
        "name": "Sample Name"
    })";
    auto [result, err] = parseJsonToStruct<Request>(missingField);
    ASSERT_FALSE(err.empty());
    for (const auto& e : err) {
        std::cout << e << std::endl;
    }
}

TEST(JsonParsingTests, IncorrectType) {
    std::string incorrectType = R"({
        "id": 12345
    })";
    auto [result, err] = parseJsonToStruct<Request>(incorrectType);
    ASSERT_FALSE(err.empty());
    for (const auto& e : err) {
        std::cout << e << std::endl;
    }
}

TEST(JsonParsingTests, UnexpectedField) {
    std::string unexpectedField = R"({
        "desc": "Unexpected field"
    })";
    auto [result, err] = parseJsonToStruct<Request>(unexpectedField);
    ASSERT_FALSE(err.empty());
    for (const auto& e : err) {
        std::cout << e << std::endl;
    }
}

TEST(JsonParsingTests, InvalidJson) {
    std::string invalidJson = R"({
        awefwafwafwafawafw
    })";
    auto [result, err] = parseJsonToStruct<Request>(invalidJson);
    ASSERT_FALSE(err.empty());
    for (const auto& e : err) {
        std::cout << e << std::endl;
    }
}

TEST(JsonParsingTests, ValidJson) {
    std::string validJson = R"({
        "id": "12345",
        "nested": {
            "name": "a",
            "value": 7
        }
    })";
    auto [result, err] = parseJsonToStruct<Request>(validJson);
    ASSERT_TRUE(err.empty()) << "Correct case failed";
    if (!err.empty()) {
        for (const auto& e : err) {
            std::cout << e << std::endl;
        }
    }
    ASSERT_NE(result, nullptr);
    std::string validationError = validate(*result);
    EXPECT_TRUE(validationError.empty()) << "Validation failed: " << validationError;
}