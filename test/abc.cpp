#include <iostream>
#include <boost/hana.hpp>
#include <string>

namespace hana = boost::hana;

// Step 1: Define the struct
struct MyStruct {
    int a;
    double b;
    std::string c;
};

// Step 2: Adapt the struct using Boost.Hana
BOOST_HANA_ADAPT_STRUCT(MyStruct, a, b, c);

// Step 3: Create a function to print the member names dynamically
template <typename T>
void print_member_names(const T& obj) {
    // Step 4: Use Hana's `members` to get the values of the struct
    auto members = hana::members(obj);

    // Step 5: Use Hana's `keys` to get the names of the struct members
    auto names = hana::keys(obj);

    // Step 6: Iterate over the names and values using Hana's `for_each`
    hana::for_each(hana::zip(names, members), [](const auto& pair) {
        auto name = hana::first(pair);  // Member name
        auto value = hana::second(pair); // Member value
        std::cout << "Member name: " << name << ", Value: " << value << std::endl;
    });
}

int main() {
    MyStruct s{42, 3.14, "Hello"};

    // Print member names and values
    print_member_names(s);

    return 0;
}
