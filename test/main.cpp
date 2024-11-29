#include <rfl/json.hpp>
#include <rfl.hpp>

int main(){
    struct Person {
        std::string first_name;
        std::string last_name = "Simpson";
        std::string town = "Springfield";
        unsigned int age;
        std::vector<Person> children;
    };

    const std::string jsonString = R"(
        {
            "first_name": "Homer",
            "last_name": "Simpson",
            "town": "Springfield",
            "age": 40,
            "children": [
                {
                    "first_name": "Bart",
                    "last_name": "Simpson",
                    "town": "Springfield",
                    "age": 10,
                    "children": []
                }
            ]
        }
    )";
    auto homer2 = rfl::json::read<Person>(jsonString).value();
    const auto view2 = rfl::to_view(homer2);
    view2.apply([](const auto& f) {
        // f is an rfl::Field pointing to the original field.
        std::cout << f.name() << ": " << rfl::json::write(*f.value()) << std::endl;
    });

    // for (const auto& f : rfl::fields<Person>()) {
    // std::cout << "name: " << f.name() << ", type: " << f.type() << std::endl;
    // }

    // auto lisa = Person{.first_name = "Lisa", .last_name = "Simpson", .age = 8};

    // const auto view = rfl::to_view(lisa);

    // view.apply([](const auto& f) {
    //     // f is an rfl::Field pointing to the original field.
    //     std::cout << f.name() << ": " << rfl::json::write(*f.value()) << std::endl;
    // });

    // // All of this is supported as well:
    // *view.get<1>() = "Simpson";
    // *view.get<"age">() = 0;
    // *rfl::get<0>(view) = "Maggie";
    // *rfl::get<"first_name">(view) = "Maggie";

    // view.apply([](const auto& f) {
    // // f is an rfl::Field pointing to the original field.
    // std::cout << f.name() << ": " << rfl::json::write(*f.value()) << std::endl;
    // });
}