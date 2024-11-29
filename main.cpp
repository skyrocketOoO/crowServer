#include "crow_all.h"
#include <json.hpp>
#include <string>
#include <iostream>
#include <type_traits>
#include <utils/parser.hpp>
#include <utils/validator.hpp>
#include "rfl.hpp"

using json = nlohmann::json;

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
        .methods("POST"_method)
        ([](const crow::request& req) {
            // using stringNotLongerThan10 = rfl::Validator<std::string, rfl::Maximum<10>>;
            // using Value = rfl::Validator<int, rfl::Minimum<0>, rfl::Maximum<10>>;
            struct Request {
                std::string id;  
                // stringNotLongerThan10 name;
                std::string name;
                struct Nested {
                    int value;
                } nested;
                
            };
            
            auto [result, error] = parseJsonToStruct<Request>(req.body);
            if (error != "") {
                return crow::response(400, error);
            }
            
            // if (std::string err = validate(*result); err != ""){
            //     return crow::response(400, err);
            // };

            const auto view = rfl::to_view(*result);
            view.apply([](const auto& f) {
                if (f.name() == "id"){
                    if (rfl::json::write(*f.value()) == "\"gg\""){
                        std::cout << f.name() << ": " << rfl::json::write(*f.value()) << std::endl;
                    }
                }
            });
            // *rfl::get<"name">(view) = "Maggie";
            // *view.get<"name">() = "Maggie";


            // Print the Nested.value field
            // std::cout << "Parsed Request: id=" << result->id 
            //           << ", name=" << result->name 
            //           << ", nested.value=" << result->nested.value << std::endl;

            return crow::response(200, "Request successfully processed");
        });

    // Run the server
    app.port(18080).multithreaded().run();
}
