#include "crow_all.h"
#include <json.hpp>
#include <string>
#include <iostream>
#include <type_traits>
#include <utils/parser.hpp>

using json = nlohmann::json;

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
        .methods("POST"_method)
        ([](const crow::request& req) {
            struct Request {
                std::string id;  
                std::string name;

                static std::vector<std::string> metadata() {
                    return {"id", "name"};
                }

                struct Nested {
                    int value;
                };
            };
            
            try {
                auto request = parseJsonToStruct<Request>(req.body);
                // Process the request further (e.g., save to database, etc.)
                std::cout << "Parsed Request: id=" << request.id << ", name=" << request.name << std::endl;
                return crow::response(200, "Request successfully processed");
            }catch (const std::exception& e) {
                // Handle any errors during parsing
                std::cout << "Error: " << e.what() << std::endl;
                return crow::response(400, "Invalid JSON format");
            }
        });

    // Run the server
    app.port(18080).multithreaded().run();
}
