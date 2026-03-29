#include <cassert>
#include <iostream>
#include <type_traits>
/*
1. std::integral_constant 怎么用？
    std::integral_constant 是一个模板，用于表示一个编译时常量值。
    它可以用于模板元编程中，作为类型和值的桥梁。

    模板参数可以接受类型参数和非类型参数，而std::integral_constant就是可以将非类型参数转换为类型参数的工具。
    相比于直接使用常量值，std::integral_constant可以更好地与模板元编程的其他工具配合使用。
···
    template<typename T>
    struct is_pointer : std::false_type {};  // 继承自 integral_constant<bool, false>

    template<typename T>
    struct is_pointer<T*> : std::true_type {}; // 继承自 integral_constant<bool, true>

    // 使用：is_pointer<int*>::value == true
···

*/

namespace integralConstantUsage {

    constexpr int value1 = std::integral_constant<int, 42>::value;
    constexpr bool value2 = std::integral_constant<bool, true>::value;

    static_assert(value1 == 42, "value1 应该是 42");
    static_assert(value2, "value2 应该是 true");

    void test() {
        std::cout << "value1: " << value1 << std::endl;
        std::cout << "value2: " << std::boolalpha << value2 << std::endl;
    }
}  // namespace integralConstantUsage

namespace integralConstantImpl {

    template <class T, T v>
    struct integral_constant {
        static constexpr T value = v;
        using value_type = T;
        using type = integral_constant;
        constexpr operator value_type() const noexcept { return value; }
    };

    using true_type = integral_constant<bool, true>;
    using false_type = integral_constant<bool, false>;

    constexpr int value1 = integral_constant<int, 42>::value;
    constexpr bool value2 = integral_constant<bool, true>::value;

    static_assert(value1 == 42, "value1 应该是 42");
    static_assert(value2, "value2 应该是 true");

    void test() {
        std::cout << "value1: " << value1 << std::endl;
        std::cout << "value2: " << std::boolalpha << value2 << std::endl;
    }
}  // namespace integralConstantImpl

namespace UnSort {

    template <class T, T v>
    struct integral_constant {
        static constexpr T value = v;
        using value_type = T;
        using type = integral_constant<T, v>;
        constexpr operator value_type() const noexcept { return value; }
        constexpr value_type operator()() const noexcept { return value; }
    };

    constexpr int to_int(char c) {
        // hexadecimal letters:
        if (c >= 'A' && c <= 'F') {
            return static_cast<int>(c) - static_cast<int>('A') + 10;
        }
        if (c >= 'a' && c <= 'f') {
            return static_cast<int>(c) - static_cast<int>('a') + 10;
        }
        assert(c >= '0' && c <= '9');
        return static_cast<int>(c) - static_cast<int>('0');
    }

    template <std::size_t N>
    constexpr int parse_int(const char (&arr)[N]) {
        int base = 10;   // to handle base (default: decimal)
        int offset = 0;  // to skip prefixes like 0x
        if (N > 2 && arr[0] == '0') {
            switch (arr[1]) {
                case 'x':  // prefix 0x or 0X, so hexadecimal
                case 'X':
                    base = 16;
                    offset = 2;
                    break;
                case 'b':  // prefix 0b or 0B (since C++14), so binary
                case 'B':
                    base = 2;
                    offset = 2;
                    break;
                default:  // prefix 0, so octal
                    base = 8;
                    offset = 1;
                    break;
            }
        }
        int res = 0;
        int multiplier = 1;
        for (std::size_t i = 0; i < N - offset; ++i) {
            if (arr[N - 1 - i] != '\'') {
                res += to_int(arr[N - 1 - i]) * multiplier;
                multiplier *= base;
            }
        }
        return res;
    }

    template <char... cs>
    constexpr auto operator"" _c() {
        return integral_constant<int, parse_int<sizeof...(cs)>({cs...})>{};
    }

    static_assert(std::is_same_v<decltype(2_c), integral_constant<int, 2>>);
    static_assert(
        std::is_same_v<decltype(0xFF_c), integral_constant<int, 255>>);
    static_assert(
        std::is_same_v<decltype(0b1111'1111_c), integral_constant<int, 255>>);

    void task() {
        static_assert(integral_constant<int, 42>::value == 42);
        static_assert(
            std::is_same_v<int, integral_constant<int, 0>::value_type>);
        static_assert(integral_constant<int, 42>{} == 42);

        integral_constant<int, 42> f;
        static_assert(f() == 42);
    }
}  // namespace UnSort

int main() {
    integralConstantUsage::test();
    integralConstantImpl::test();
    return 0;
}