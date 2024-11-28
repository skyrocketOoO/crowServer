#include "crow_all.h"
#include <json.hpp>
#include <optional>
#include <vector>
#include <string>
#include <iostream>

using json = nlohmann::json;

// Metadata structure
struct FieldMeta {
    std::string fieldName;
    std::vector<std::string> rules;
};

// Generic validation function
template <typename T>
bool validate(const T& obj) {
    for (const auto& meta : T::metadata()) {
        const std::string& fieldName = meta.fieldName;
        const auto& rules = meta.rules;

        // Extract field value using the metadata field name
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

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/add_json")
        .methods("POST"_method)
        ([](const crow::request& req) {
            struct Request {
                std::string id;  // REQUIRED, MAX_LENGTH(10)
                std::string name; // MAX_LENGTH(20)

                static std::vector<FieldMeta> metadata() {
                    return {
                        {"id", {"required", "maxLength:10"}},
                        {"name", {"maxLength:20"}}
                    };
                }
            };

            // Parse the JSON body into a Request object
            Request request;
            try {
                auto body = json::parse(req.body, nullptr, false);
                if (body.is_discarded()) {
                    return crow::response(400, "Invalid JSON");
                }

                request.id = body.at("id").get<std::string>();
                request.name = body.at("name").get<std::string>();
            } catch (const std::exception& e) {
                return crow::response(400, "Missing or invalid fields");
            }

            // Validate the request object
            if (!validate(request)){
              return crow::response(400, "Validate false");
            };

            // Add additional logic here (e.g., save to database, process, etc.)
            return crow::response(200, "Validation passed and request processed");
        });

    app.port(18080).multithreaded().run();
}
