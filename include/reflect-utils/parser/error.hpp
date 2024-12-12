#ifndef PARSERERROR_HPP
#define PARSERERROR_HPP

#include <string>
#include <variant>
#include <regex>
#include "rfl.hpp"
#include "rfl/json.hpp"

enum class ErrorType {
    MissingField,
    IncorrectType,
    UnknownField,
    InvalidJson,
    UnknownError
};

struct ErrorMissingField {
    ErrorType type = ErrorType::MissingField;
    std::string fieldName;
};

struct ErrorIncorrectType {
    ErrorType type = ErrorType::IncorrectType;
    std::string fieldName;
    std::string fromType;
    std::string toType;
};

struct ErrorUnknownField {
    ErrorType type = ErrorType::UnknownField;
    std::string fieldName;
};

struct ErrorInvalidJson {
    ErrorType type = ErrorType::InvalidJson;
};

struct ErrorUnknown {
    ErrorType type = ErrorType::UnknownError;
    std::string errorMessage;
};

using Error = std::variant<ErrorMissingField, ErrorIncorrectType, ErrorUnknownField, ErrorInvalidJson, ErrorUnknown>;

// Function to map error messages to error structs
std::string mapErrors(const std::string& errorMessage) {
    // Regex patterns for different error types
    std::regex missingFieldRegex(R"(Field named '(\w+)' not found\.)");
    std::regex incorrectTypeRegex(R"(Failed to parse field '(\w+)': Could not cast to (\w+)\.)");
    std::regex unknownFieldRegex(R"(Value named '(\w+)' not used\.)");
    std::regex invalidJsonRegex(R"(Could not parse document)");

    std::smatch match;

    Error err;
    if (std::regex_search(errorMessage, match, missingFieldRegex)) {
        err = ErrorMissingField{ErrorType::MissingField, match[1]};
    } else if (std::regex_search(errorMessage, match, incorrectTypeRegex)) {
        err = ErrorIncorrectType{ErrorType::IncorrectType, match[1], match[2], "string"}; // Assuming "string" as target type
    } else if (std::regex_search(errorMessage, match, unknownFieldRegex)) {
        err = ErrorUnknownField{ErrorType::UnknownField, match[1]};
    } else if (std::regex_search(errorMessage, invalidJsonRegex)) {
        err = ErrorInvalidJson{ErrorType::InvalidJson};
    } else {
        err = ErrorUnknown{ErrorType::UnknownError, errorMessage};
    }

    return rfl::json::write(err);
}

#endif //PARSERERROR_HPP