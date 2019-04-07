//
// Created by wangrl on 19-4-4.
//

#include <optional>
#include <iostream>
#include <string>
#include <functional>

// optional can be used as the return type of a factory that may fail.
std::optional<std::string> create(bool b) {
    if (b)
        return "Godzilla";
    return {};
}

// std::nullopt can be used to create any (empty) std::optional.
auto create2(bool b) {
    return b ? std::optional<std::string>{"Godzilla"} : std::nullopt;
}

// std::reference_wrapper may be used to return a reference
auto create_ref(bool b) {
    static std::string value = "Godzilla";
    return b ? std::optional<std::reference_wrapper<std::string>>{value}
             : std::nullopt;
}

int main(int argc, char* argv[]) {

    std::cout << "create(false) returned " << create(false).value_or("empty") << '\n';

    // optional-returning factory functions are usable as conditions of while and if.
    if (auto str = create2(true)) {
        // 需要用*的方式取出。
        std::cout << "create2(true) return " << *str << '\n';
    }

    if (auto str = create_ref(true)) {
        // using get() to access the reference_wrapper's value
        std::cout << "create_ref(true) returned " << str->get() << '\n';
        str->get() = "Mothra";
        std::cout << "modifying it changed it to " << str->get() << '\n';
    }

    return 0;
}

