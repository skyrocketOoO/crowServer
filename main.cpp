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
                struct Nested {
                    int value;
                } nested;
                
            };
            
            auto [result, error] = parseJsonToStruct<Request>(req.body);
            if (error != "") {
                return crow::response(400, error);
            }
            
            // Print the Nested.value field
            std::cout << "Parsed Request: id=" << result->id 
                      << ", name=" << result->name  
                      << ", nested.value=" << result->nested.value << std::endl;



            return crow::response(200, "Request successfully processed");
        });

    // Run the server
    app.port(18080).multithreaded().run();
}
