#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <iostream>
#include "reflect-utils/parser/parser.hpp"
#include "reflect-utils/parser/error.hpp"
#include "reflect-utils/validator/validator.hpp"
#include "reflect-utils/validator/rule.hpp"
#include "rfl.hpp"
#include "rfl/json.hpp"

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

TEST(Parser, NotWritable) {
    std::string missingField = R"({
        "name": "Sample Name"
    })";
    auto [result, err] = parseJsonToStruct<Request>(missingField);
    ASSERT_FALSE(err.empty());
    for (auto v : err){
      std::cout << v << std::endl;
    }
}

TEST(Parser, MissingRequiredField) {
    std::string missingField = R"({
        "id": "12345",
        "name": "Sample Name"
    })";
    auto [result, err] = parseJsonToStruct<Request>(missingField);
    ASSERT_FALSE(err.empty());
    const auto errSt = rfl::json::read<ErrorMissingField>(err[0]).value();
    ASSERT_EQ(errSt.type, ErrorType::MissingField);
}

TEST(Parser, IncorrectType) {
    std::string incorrectType = R"({
        "id": 12345
    })";
    auto [result, err] = parseJsonToStruct<Request>(incorrectType);
    ASSERT_FALSE(err.empty());
}

TEST(Parser, UnexpectedField) {
    std::string unexpectedField = R"({
        "desc": "Unexpected field"
    })";
    auto [result, err] = parseJsonToStruct<Request>(unexpectedField);
    ASSERT_FALSE(err.empty());
}

TEST(Parser, InvalidJson) {
    std::string invalidJson = R"({
        awefwafwafwafawafw
    })";
    auto [result, err] = parseJsonToStruct<Request>(invalidJson);
    ASSERT_FALSE(err.empty());
}

TEST(Parser, ValidJson) {
    std::string validJson = R"({
        "id": "12345",
        "nested": {
            "name": "a",
            "value": 11
        }
    })";
    auto [result, err] = parseJsonToStruct<Request>(validJson);
    ASSERT_TRUE(err.empty()) << "Correct case failed";
    ASSERT_NE(result, nullptr);
    std::string validationError = validate(*result);
    EXPECT_TRUE(validationError.empty()) << "Validation failed: " << validationError;
}