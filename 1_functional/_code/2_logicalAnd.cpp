#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>

namespace {

    bool begins_with_a(const std::string& s) { return s.find('a') == 0; }
    bool ends_with_b(const std::string& s) {
        return s.rfind('b') == s.length() - 1;
    }
}  // namespace

template <typename A, typename B, typename F>
auto combine(F binary_func, A a, B b) {
    return [=](auto param) { return binary_func(a(param), b(param)); };
}

int main() {
    auto a_xxx_b(combine(std::logical_and<>{}, begins_with_a, ends_with_b));

    std::copy_if(std::istream_iterator<std::string>{std::cin}, {},
                 std::ostream_iterator<std::string>{std::cout, ", "}, a_xxx_b);
    std::cout << '\n';
    return 0;
}