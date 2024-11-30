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
            struct Request {
                std::string id;
                std::string name;
                struct Nested {
                    int value;

                    auto metadata() {
                        return std::array{
                            FieldMeta("value", {Rule::Required()}),
                        };
                    }
                } nested;
                

                auto metadata() {
                    return std::array{
                        FieldMeta("id", {Rule::Required()}),
                        FieldMeta("name", {Rule::Required(), Rule::String::MaxLength(10)}),
                    };
                }
            };
            
            auto [result, error] = parseJsonToStruct<Request>(req.body);
            if (error != "") {
                return crow::response(400, error);
            }

            if (std::string err = validate(*result); err != ""){
                return crow::response(400, err);
            };

            return crow::response(200, "Request successfully processed");
        });

    // Run the server
    app.port(18080).multithreaded().run();
}
