#include <iostream>
#include <json.hpp>
#include <map>
#include <variant>
#include <string>
#include <stdexcept>  // For exception handling

// Define Field Structs
struct IntField {
    int value;
    std::string type = "int";
};

struct StringField {
    std::string value;
    std::string type = "string";
};

// Define TypeField as a variant of different types of fields
using TypeField = std::variant<IntField, StringField>;

// Function to print the field (Type + Value)
void printField(const TypeField& field) {
    std::visit([](auto&& arg) {
        std::cout << "Type: " << arg.type << ", Value: ";
        if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, IntField>) {
            std::cout << arg.value;
        } else if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, StringField>) {
            std::cout << arg.value;
        }
        std::cout << std::endl;
    }, field);
}

// Function to print the type of the current object in the variant
template<typename T>
void print_type(const T&) {
    std::cout << "Type: " << typeid(T).name() << std::endl;
}

// Function to handle variant type assignments safely
void updateFieldValue(TypeField& field, const nlohmann::json& j_value) {
    std::visit([&j_value](auto&& arg) {
        if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, IntField>) {
            if (j_value.is_number()) {
                arg.value = j_value.get<int>();  // Assign value if it's an IntField and the JSON value is a number
            } else {
                throw std::invalid_argument("Attempted to assign a non-integer value to an IntField.");
            }
        } else if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, StringField>) {
            if (j_value.is_string()) {
                arg.value = j_value.get<std::string>();  // Assign value if it's a StringField and the JSON value is a string
            } else {
                throw std::invalid_argument("Attempted to assign a non-string value to a StringField.");
            }
        }
    }, field);
}

int main() {
    // Initialize map with default values
    std::map<std::string, TypeField> myMap = {
        {"x", IntField{42}},
        {"y", StringField{"3.14"}},
        {"z", StringField{"Hello"}},
        {"x_invalid", IntField{1}}
    };

    // Example JSON object with valid and invalid types
    nlohmann::json j = {
        {"x", 42},    // Valid for IntField
        {"y", "abc"}, // Valid for StringField
        {"z", "World"}, // Valid for StringField
        {"x_invalid", "Not an int"} // Invalid, should trigger an error
    };

    // Update the map based on the JSON
    for (auto& [k, v] : j.items()) {
        auto it = myMap.find(k);
        if (it != myMap.end()) {
            try {
                // Use the function to safely update the value in the variant
                updateFieldValue(it->second, v);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: " << e.what() << " for key: " << k << std::endl;
            }
        }
    }

    // Print updated map contents
    for (const auto& [key, value] : myMap) {
        std::cout << key << ": ";
        printField(value);
    }

    return 0;
}
