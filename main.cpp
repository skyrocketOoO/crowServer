

#include <gtest/gtest.h>
#include "json.hpp"
#include <variant>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>

// Define JsonValue as a variant of possible JSON types
using JsonValue = std::variant<
    std::string,                                // String
    double,                                     // Number (floating-point)
    int,                                        // Number (integer)
    bool                                       // Boolean
>;

using json = nlohmann::json;
void from_json(const json& j, JsonValue& value) {
    if (j.is_string()) {
        value = j.get<std::string>();
    } else if (j.is_number_float()) {
        value = j.get<double>();
    } else if (j.is_number_integer()) {
        value = j.get<int>();
    } else if (j.is_boolean()) {
        value = j.get<bool>();
    } else {
        throw std::runtime_error("Unsupported JSON type");
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << RUN_ALL_TESTS() << std::endl;


    // std::string input = R"(
    //     true
    // )";

    // json j = json::parse(input);

    // JsonValue value;

    // try {
    //     from_json(j, value);

    //     std::visit([](const auto& v) { 
    //         std::cout << "Parsed value: " << v << std::endl; 
    //     }, value);
    // } catch (const std::exception& e) {
    //     std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    // }

    return 0;
}
