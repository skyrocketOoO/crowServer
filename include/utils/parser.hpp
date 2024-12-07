#include "rfl/json.hpp"
#include "rfl.hpp"
#include "rfl/Processors.hpp"
#include <variant>
#include <regex>

// Function to split error messages into individual errors
std::vector<std::string> splitErrors(const std::string& errorMessage) {
    std::vector<std::string> errors;
    std::istringstream stream(errorMessage);
    std::string line;

    while (std::getline(stream, line, '\n')) {
        if (!line.empty()) {
            errors.push_back(line);
        }
    }

    if (errors.size() > 1){
        return std::vector<std::string>(errors.begin() + 1, errors.end());
    }
    return errors;
}


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

template <typename T>
std::pair<T*, std::vector<Error>> parseJsonToStruct(const std::string& jsonStr) {
    try {
        auto parsedResult = rfl::json::read<T, rfl::NoExtraFields>(jsonStr);
        return {new T(parsedResult.value()), {}};  // Return a new object on success
    } catch (const std::exception& e) {
        std::string errMsg = e.what();
        auto errStrs = splitErrors(errMsg);
        std::vector<Error> errs;
        for (auto errStr : errStrs){
            // std::cout << err << std::endl;
            auto errSt = mapErrors(errStr);
            // std::visit([](const auto& err) {
            //     std::cout << static_cast<int>(err.type) << std::endl;
            // }, errSt);
            errs.push_back(errSt);
        }

        
        
        return {nullptr, errs};
        // return {nullptr, e.what()};  // Return nullptr and the error message
    }
}