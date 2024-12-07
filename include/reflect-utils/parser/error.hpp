#include <string>
#include <variant>
#include <regex>


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
Error mapErrors(const std::string& errorMessage) {
    std::vector<Error> errors;

    // Regex patterns for different error types
    std::regex missingFieldRegex(R"(Field named '(\w+)' not found\.)");
    std::regex incorrectTypeRegex(R"(Failed to parse field '(\w+)': Could not cast to (\w+)\.)");
    std::regex unknownFieldRegex(R"(Value named '(\w+)' not used\.)");
    std::regex invalidJsonRegex(R"(Could not parse document)");

    std::smatch match;

    if (std::regex_search(errorMessage, match, missingFieldRegex)) {
        return ErrorMissingField{ErrorType::MissingField, match[1]};
    } else if (std::regex_search(errorMessage, match, incorrectTypeRegex)) {
        return ErrorIncorrectType{ErrorType::IncorrectType, match[1], match[2], "string"}; // Assuming "string" as target type
    } else if (std::regex_search(errorMessage, match, unknownFieldRegex)) {
        return ErrorUnknownField{ErrorType::UnknownField, match[1]};
    } else if (std::regex_search(errorMessage, invalidJsonRegex)) {
        return ErrorInvalidJson{ErrorType::InvalidJson};
    } else {
        return ErrorUnknown{ErrorType::UnknownError, errorMessage};
    }

    return ErrorUnknown{ErrorType::UnknownError, errorMessage};  // Return default error if no pattern matches
}