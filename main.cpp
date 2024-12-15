

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
using json = nlohmann::json;

template <typename T, typename Field>
concept HasMember = requires(T t) {
    { t.*Field() };  // Check if we can access the member field using T.*Field()
};

std::string demangle(const char* name) {
    int status = 0;
    std::unique_ptr<char[], void(*)(void*)> demangled(
        abi::__cxa_demangle(name, nullptr, nullptr, &status),
        std::free
    );
    return (status == 0) ? demangled.get() : name;
}

std::string type_to_string(json::value_t type) {
    switch (type) {
        case json::value_t::null: return "null";
        case json::value_t::boolean: return "boolean";
        case json::value_t::number_integer: return "integer";
        case json::value_t::number_unsigned: return "unsigned integer";
        case json::value_t::number_float: return "float";
        case json::value_t::string: return "string";
        case json::value_t::object: return "object";
        case json::value_t::array: return "array";
        case json::value_t::binary: return "binary";
        case json::value_t::discarded: return "discarded";
        default: return "unknown type";
    }
}


int main(int argc, char** argv) {
    // ::testing::InitGoogleTest(&argc, argv);
    // std::cout << RUN_ALL_TESTS() << std::endl;

    crow::SimpleApp app;

    CROW_ROUTE(app, "/").methods("POST"_method)([](const crow::request& req){
        struct Request {
            std::string id;
            std::string name;
            struct Nested {
                std::string value;
            } nested;
        };
        json j = json::parse(req.body);

        Request request;
        auto ids = std::make_index_sequence<rfl::internal::num_fields<Request>>();
        const auto view = rfl::to_view(request);


        for (auto it : j.items()){
            const std::string& key = it.key();
            // std::cout << key << std::endl;
            auto get = [key, view, it]<std::size_t... _is>(std::index_sequence<_is...>) {
                (
                    [&]() {
                        auto field = rfl::internal::get_field_name<
                            std::remove_cvref_t<Request>,
                            rfl::internal::get_ith_field_from_fake_object<Request, _is>()
                        >();
                        if (field.name() == key) {
                            std::cout << "field: " << demangle(typeid(field).name()) << std::endl;
                            std::cout << "key: " << type_to_string(it.value().type()) << std::endl;
                            // *rfl::get<_is>(view) = it.value();
                        }
                    }(),
                    ...
                );
            }; 
            get(std::make_index_sequence<rfl::internal::num_fields<Request>>());
        }

        std::cout << request.id << std::endl;
        std::cout << request.name << std::endl;


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
