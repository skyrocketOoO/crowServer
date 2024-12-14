

#include <gtest/gtest.h>
#include "json.hpp"
#include <variant>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include <type_traits>
#include "rfl/json.hpp"
#include "rfl/internal/to_ptr_named_tuple.hpp"
#include "rfl/internal/num_fields.hpp"
#include "rfl.hpp"
#include <concepts>
#include <array>
#include <string_view>
#include "crow_all.h"

template <typename T, typename Field>
concept HasMember = requires(T t) {
    { t.*Field() };  // Check if we can access the member field using T.*Field()
};


int main(int argc, char** argv) {
    // ::testing::InitGoogleTest(&argc, argv);
    // std::cout << RUN_ALL_TESTS() << std::endl;

    // std::string jsonStr = R"({
    //     "id": "12345",
    //     "name": "Sample Name"
    // })";
    // nlohmann::json j = nlohmann::json::parse(jsonStr);

    crow::SimpleApp app;

    CROW_ROUTE(app, "/").methods("POST"_method)([](const crow::request& req){
        struct Request {
            std::string id;
            std::string name;
        };
        using json = nlohmann::json;
        json j = json::parse(req.body);

        Request request;
        auto ids = std::make_index_sequence<rfl::internal::num_fields<Request>>();
        const auto view = rfl::to_view(request);



        for (auto it : j.items()){
            const std::string& key = it.key();
            // std::cout << key << std::endl;
            auto get = [key, view, it]<std::size_t... _is>(std::index_sequence<_is...>) {
                // Use a fold expression over initializer_list to iterate and check field names
                (void)std::initializer_list<int>{
                    (
                        [key, view, it]() {
                            auto name = rfl::internal::get_field_name<
                                std::remove_cvref_t<Request>,
                                rfl::internal::get_ith_field_from_fake_object<Request, _is>()
                            >().name();
                            // std::cout << name << std::endl;
                            if (name == key) {
                                *rfl::get<_is>(view) = it.value();
                                std::cout << name << std::endl;
                            }

                        }(),
                        0 // Fold expression placeholder
                    )...
                };
            }; 
            get(std::make_index_sequence<rfl::internal::num_fields<Request>>());
        }

        std::cout << request.id << std::endl;
        std::cout << request.name << std::endl;

        
        // const auto get = []<std::size_t... _is>(std::index_sequence<_is...>) {
        //     // Use fold expression to expand the parameter pack and call get_field_name for each index
        //     (void)std::initializer_list<int>{
        //         ((std::cout << rfl::internal::get_field_name<
        //             std::remove_cvref_t<Request>,
        //             rfl::internal::get_ith_field_from_fake_object<Request, _is>() // Correct usage
        //         >().name() << std::endl), 0)...};  // Print each field name
        // };

        // get(std::make_index_sequence<rfl::internal::num_fields<Request>>());

        // rfl::internal::get_field_name<std::remove_cvref_t<Request>, rfl::internal::get_ith_field_from_fake_object<Request, 1>()>();
        // std::cout << name.name() << std::endl;


        return crow::response(200);
    });

    app.port(18080).multithreaded().run();


    // Request req;
    // req.id = "10";
    // req.name = "John Doe";
    // const auto view = rfl::to_view(req);
    // view.apply([](const auto& f) {
    // // f is an rfl::Field pointing to the original field.
    // std::cout << f.name() << ": " << rfl::json::write(*f.value()) << std::endl;
    // });
    
    // view.apply([j, view](const auto &f){
    //     for (auto it: j.items()){
    //         const std::string& key = it.key();
    //         if (f.name() == key){
    //             // f.set(it.value());
    //             *rfl::get<f.name()>(view) = "Maggie";
    //             // return f;
    //             std::cout << it.key() << std::endl;
    //         }
    //     }
    // });

    // std::cout << "id: " << req.id << ", name: " << req.name << std::endl;


    return 0;
}
