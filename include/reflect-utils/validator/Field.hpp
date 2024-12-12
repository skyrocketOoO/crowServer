#pragma once

template <typename T>
struct Field {
    std::string name;
    std::vector<std::function<std::string(std::string,T)>> validateFuncs;

    T castAny(const std::any& value) const {
        try {
            return std::any_cast<T>(value);
        } catch (const std::bad_any_cast&) {
            throw std::runtime_error("Type mismatch: failed to cast std::any to the required type.");
        }
    }

    // TODO: Add static check to avoid the T is not fit the field type
};