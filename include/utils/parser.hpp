#include "rfl/json.hpp"
#include "rfl.hpp"
#include "rfl/Processors.hpp"


template <typename T>
std::pair<T*, std::string> parseJsonToStruct(const std::string& jsonStr) {
    try {
        auto parsedResult = rfl::json::read<T>(jsonStr);
        return {new T(parsedResult.value()), ""};  // Return a new object on success
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return {nullptr, e.what()};  // Return nullptr and the error message
    }
}