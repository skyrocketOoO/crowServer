#include "crow_all.h"
#include <json.hpp>
#include <string>
#include <iostream>
#include <type_traits>

using json = nlohmann::json;

// Utility function to parse simple types (non-struct fields)
template <typename T>
void parseField(T& obj, const std::string& key, const json& body) {
    if (body.contains(key)) {
        obj = body.at(key).get<T>();  // Assign the value directly
    }
}

// Specialization for nested structs: Recursively handle nested fields
template <typename T>
void parseNestedStruct(T& obj, const std::string& key, const json& body) {
    if (body.contains(key)) {
        // Extract the nested JSON object for this key
        auto nestedJson = body.at(key);
        obj = parseRequest<typename T::nested_struct_type>(nestedJson);  // Recursively parse the nested struct
    }
}

// General recursive parser function to map JSON fields to struct fields
template <typename T>
T parseRequest(const json& body) {
    T obj;

    // Iterate through JSON keys and map to struct fields
    for (auto& [key, value] : body.items()) {
        if constexpr (std::is_class_v<T>) {
            if constexpr (std::is_class_v<decltype(obj.*(&T::id))>) {  // Using SFINAE to check if field is a nested struct
                parseNestedStruct(obj, key, body);  // Recurse for nested struct
            } else {
                parseField(obj, key, body);  // Otherwise, treat it as a simple field
            }
        } else {
            // If the field is a primitive type, assign it directly
            parseField(obj, key, body);
        }
    }

    return obj;
}

int main() {
    crow::SimpleApp app;

    // Define the Request struct
    struct Request {
        std::string id;  // REQUIRED, MAX_LENGTH(10)
        std::string name; // MAX_LENGTH(20)

        // Optional metadata for validation
        static std::vector<std::string> metadata() {
            return {"id", "name"};
        }

        // Example of a nested struct
        struct Nested {
            int value;
        };
        using nested_struct_type = Nested;
    };

    // Define the route to handle POST requests
    CROW_ROUTE(app, "/add_json")
        .methods("POST"_method)
        ([](const crow::request& req) {
            // Parse the JSON body into a Request object
            Request request;
            try {
                // Parse the request body to a nlohmann::json object
                auto body = json::parse(req.body, nullptr, false);
                if (body.is_discarded()) {
                    throw std::runtime_error("Invalid JSON");
                }

                // Map the JSON body to the Request struct
                request = parseRequest<Request>(body);  // Call the parser function with the parsed JSON body
            } catch (const std::exception& e) {
                return crow::response(400, "Missing or invalid fields");
            }

            // Process the request further (e.g., save to database, etc.)
            std::cout << "Parsed Request: id=" << request.id << ", name=" << request.name << std::endl;
            return crow::response(200, "Request successfully processed");
        });

    // Run the server
    app.port(18080).multithreaded().run();
}
