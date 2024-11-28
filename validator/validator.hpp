// Validator.hpp
#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

#include <vector>
#include <string>
#include <iostream>

// Define the FieldMeta structure
struct FieldMeta {
    std::string fieldName;
    std::vector<std::string> rules;
};

// Define Rule namespace for validation rules
namespace Rule {
    std::string Required();
    std::string MaxLength(int val);
}

// Generic validation function template
template <typename T>
bool validate(const T& obj) {
    for (const auto& meta : T::metadata()) {
        const std::string& fieldName = meta.fieldName;
        const auto& rules = meta.rules;

        // Extract field value using metadata field name
        std::string fieldValue;
        if (fieldName == "id") {
            fieldValue = obj.id;
        } else if (fieldName == "name") {
            fieldValue = obj.name;
        }
        // Add checks for more fields as needed.

        // Validate rules
        for (const auto& rule : rules) {
            if (rule == "required" && fieldValue.empty()) {
                std::cerr << "Error: " << fieldName << " is required.\n";
                return false;
            }
            if (rule.find("maxLength:") == 0) {
                size_t maxLen = std::stoul(rule.substr(10)); // Extract max length
                if (fieldValue.size() > maxLen) {
                    std::cerr << "Error: " << fieldName
                              << " exceeds max length of " << maxLen << ".\n";
                    return false;
                }
            }
        }
    }
    return true;
}

#endif // VALIDATOR_HPP
