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
#include <variant>




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

TEST(Parser, MissingRequiredField) {
    std::string req = R"({
        "id": "12345",
        "name": "Sample Name"
    })";
    auto [result, err] = Parser::Read<Request>(req);
    ASSERT_FALSE(err.empty());
    try {
        const auto errSt = rfl::json::read<Parser::ErrorMissingField>(err[0]).value();
        ASSERT_EQ(errSt.type, Parser::ErrorType::MissingField);
    }catch (const std::exception& e){
        std::cout << e.what() << std::endl;
    }
}

TEST(Parser, IncorrectType) {
    std::string req = R"({
        "id": 12345
    })";
    auto [result, err] = Parser::Read<Request>(req);
    ASSERT_FALSE(err.empty());
    try {
        const auto errSt = rfl::json::read<Parser::ErrorIncorrectType>(err[0]).value();
        ASSERT_EQ(errSt.type, Parser::ErrorType::IncorrectType);
    }catch (const std::exception& e){
        std::cout << e.what() << std::endl;
    }
}

TEST(Parser, UnexpectedField) {
    std::string req = R"({
        "desc": "Unexpected field"
    })";
    auto [result, err] = Parser::Read<Request>(req);
    ASSERT_FALSE(err.empty());
    try {
        const auto errSt = rfl::json::read<Parser::ErrorUnknownField>(err[0]).value();
        ASSERT_EQ(errSt.type, Parser::ErrorType::UnknownField);
    }catch (const std::exception& e){
        std::cout << e.what() << std::endl;
    }
}

TEST(Parser, InvalidJson) {
    std::string req = R"({
        awefwafwafwafawafw
    })";
    auto [result, err] = Parser::Read<Request>(req);
    ASSERT_FALSE(err.empty());
    try {
        const auto errSt = rfl::json::read<Parser::ErrorInvalidJson>(err[0]).value();
        ASSERT_EQ(errSt.type, Parser::ErrorType::InvalidJson);
    }catch (const std::exception& e){
        std::cout << e.what() << std::endl;
    }
}

TEST(Parser, ValidJson) {
    std::string req = R"({
        "id": "12345",
        "nested": {
            "name": "a",
            "value": 11
        }
    })";
    auto [result, err] = Parser::Read<Request>(req);
    ASSERT_TRUE(err.empty()) << "Correct case failed";
    ASSERT_NE(result, nullptr);
}

TEST(Validator, Nested) {
    std::string req = R"({
        "id": "12345",
        "nested": {
            "name": "a",
            "value": 9
        }
    })";
    auto [result, err] = Parser::Read<Request>(req);
    ASSERT_TRUE(err.empty()) << "Correct case failed";
    ASSERT_NE(result, nullptr);
    std::string validationError = validate(*result);
    EXPECT_FALSE(validationError.empty());
}

TEST(Validator, Or) {
    std::string req = R"({
        "id": "12345",
        "nested": {
            "name": "a",
            "value": 9
        }
    })";
    auto [result, err] = Parser::Read<Request>(req);
    ASSERT_TRUE(err.empty()) << "Correct case failed";
    ASSERT_NE(result, nullptr) << "Correct case failed";
    std::string validationError = validate(*result);
    EXPECT_FALSE(validationError.empty());
}

TEST(Validator, In) {
    std::string req = R"({
        "id": "12345",
        "nested": {
            "name": "c",
            "value": 9
        }
    })";
    auto [result, err] = Parser::Read<Request>(req);
    ASSERT_TRUE(err.empty()) << "Correct case failed";
    ASSERT_NE(result, nullptr) << "Correct case failed";
    std::string validationError = validate(*result);
    EXPECT_FALSE(validationError.empty());
}

TEST(Validator, NotWritable) {
    std::string req = R"({
        "id": "12345",
        "name": "gg",
        "nested": {
            "name": "a",
            "value": 9
        }
    })";
    auto [result, err] = Parser::Read<Request>(req);
    ASSERT_TRUE(err.empty()) << "Correct case failed";
    ASSERT_NE(result, nullptr) << "Correct case failed";
    std::string validationError = validate(*result);
    EXPECT_FALSE(validationError.empty());
}