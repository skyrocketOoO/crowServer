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
                std::optional<std::string> name;

                struct Nested {
                    int value;

                    auto metadata() {
                        return std::array{
                            FieldMeta("value", {Rule::Number::Min(0)}),
                        };
                    }
                } nested;
                
                // auto metadata() {
                //     return std::array{
                //         FieldMeta("name", {Rule::String::MaxLen(10)}),
                //     };
                // }
            };
            
            auto [result, err] = parseJsonToStruct<Request>(req.body);
            if (err != "") {
                return crow::response(400, "parse" + err);
            }

            if (std::string err = validate(*result); err != ""){
                return crow::response(400, "validate" + err);
            };

            return crow::response(200, "Request successfully processed");
        });

    app.port(18080).multithreaded().run();
}
