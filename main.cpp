#include "crow_all.h"
#include <json.hpp>
#include <optional>
#include <vector>
#include <string>
#include <iostream>

using json = nlohmann::json;

struct FieldMeta {
    std::string fieldName;
    std::vector<std::string> rules;
};

struct Request {
    std::string id;  // REQUIRED, MAX_LENGTH(10)
    std::string name; // MAX_LENGTH(20)

    // Metadata for validation rules
    static std::vector<FieldMeta> metadata() {
        return {
            {"id", {"required", "maxLength:10"}},
            {"name", {"maxLength:20"}}
        };
    }
};

// Validation function
void validate(const Request& req) {
    for (const auto& meta : Request::metadata()) {
        const std::string& fieldName = meta.fieldName;
        const auto& rules = meta.rules;

        // Validate based on rules
        for (const auto& rule : rules) {
            if (rule == "required" && fieldName == "id" && req.id.empty()) {
                std::cerr << "Error: " << fieldName << " is required.\n";
            }
            if (rule.find("maxLength:") == 0 && fieldName == "id") {
                size_t maxLen = std::stoul(rule.substr(10)); // Extract max length
                if (req.id.size() > maxLen) {
                    std::cerr << "Error: " << fieldName
                              << " exceeds max length of " << maxLen << ".\n";
                }
            }
            if (rule.find("maxLength:") == 0 && fieldName == "name") {
                size_t maxLen = std::stoul(rule.substr(10)); // Extract max length
                if (req.name.size() > maxLen) {
                    std::cerr << "Error: " << fieldName
                              << " exceeds max length of " << maxLen << ".\n";
                }
            }
        }
    }
}

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        return "Hello world";
    });

    CROW_ROUTE(app, "/add_json")
        .methods("POST"_method)
        ([](const crow::request& req) {
            auto body = json::parse(req.body, nullptr, false);
            if (body.is_discarded()) {
                return crow::response(400, "Invalid JSON");
            }

            Request request;
            try {
                request.id = body.at("id").get<std::string>();
                request.name = body.at("name").get<std::string>();
            } catch (const std::exception& e) {
                return crow::response(400, "Missing required fields");
            }

            // Validate the parsed request
            validate(request);

            return crow::response(200, "Validation completed");
        });

    app.port(18080).multithreaded().run();
}
