struct Request {
    std::string id;
    std::optional<std::string> name;

    struct Nested {
        std::string name;
        int value;

        auto validateMetas() {
            return std::tuple{
                Field<int>{"value", {Rule::Number::Min(0), Rule::Number::Max(20)}},
                Field<std::string>{"name", {Rule::String::MaxLen(10)}},
            };
        }
    } nested;
    
    auto validateMetas() {
        return std::tuple{
            Field<std::string>{"name", {Rule::String::MaxLen(10)}},
        };
    }
};