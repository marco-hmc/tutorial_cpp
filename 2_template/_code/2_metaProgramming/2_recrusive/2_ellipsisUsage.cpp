#include <stdarg.h>
#include <stdio.h>

#include <cassert>
#include <iostream>
#include <numeric>
#include <tuple>

namespace CStyleVariadic {
    void printNumbers(int count, ...) {
        va_list args;
        va_start(args, count);
        for (int i = 0; i < count; i++) {
            int num = va_arg(args, int);
            printf("%d ", num);
        }

        va_end(args);
    }
}  // namespace CStyleVariadic

namespace C11VariadicRecursive {
    namespace example_1 {
        int sum() { return 0; }

        template <typename T, typename... Args>
        T sum(T first, Args... args) {
            return first + sum(args...);
        }
    }  // namespace example_1

    namespace example_2 {
        template <typename T0>
        void printf1(T0 value) {
            std::cout << value << '\n';
        }
        template <typename T, typename... Ts>
        void printf1(T value, Ts... args) {
            std::cout << value << '\n';
            printf1(args...);
        }
    }  // namespace example_2

    namespace example_3 {

        void print() {}

        template <typename T, typename... Args>
        void print(const T &t, Args &&...args) {
            std::cout << t << ",";
            print(std::forward<Args>(args)...);
        }

        template <int... Index>
        struct A {};

        template <typename... List, int... Index>
        void test1(const std::tuple<List...> &t, A<Index...>) {
            // print(std::get<2>(t), std::get<3>(t));
            print(std::get<Index>(t)...);
        }

        template <typename... List, int... Index>
        void test2(const std::tuple<List...> &t, A<Index...>) {
            print((std::get<Index>(t) + std::get<Index>(t))...);
        }

        void test() {
            auto t = std::make_tuple(3.14, 42, std::string{"hello"}, "world");
            test1(t, A<2, 3>{});     // hello,world
            test2(t, A<0, 1, 2>{});  // 6.28,84,hellohello,
        }
    }  // namespace example_3

    void test() { assert(example_1::sum(1, 2, 3, 4, 5) == 15); }
}  // namespace C11VariadicRecursive

namespace C11VariadicInitializerList {
    template <typename First, typename... Args>
    auto sum(const First first, const Args... args) -> decltype(first) {
        const auto values = {first, args...};
        return std::accumulate(values.begin(), values.end(), First{0});
    }

    void test() { assert(sum(1, 2, 3, 4, 5) == 15); }

    template <typename T, typename... Ts>
    auto printf3(T value, Ts... args) {
        std::cout << value << '\n';
        (void)std::initializer_list<T>{
            ([&args] { std::cout << args << '\n'; }(), value)...};
    }
}  // namespace C11VariadicInitializerList

namespace Cpp17FoldExpressionForSum {
    namespace example_1 {
        template <typename... Args>
        auto sum(Args... args) {
            return (args + ...);
        }
    }  // namespace example_1

    namespace example_2 {
        template <typename... Args>
        auto sum(Args &&...args) {
            auto a = (... + std::forward<Args>(args));  // (((1 + 2) + 3) + 4)
            auto b = (std::forward<Args>(args) + ...);  // (1 + (2 + (3 + 4)))
            auto c = (5 + ... +
                      std::forward<Args>(args));  // ((((5 + 1) + 2) + 3) + 4)
            auto d = (std::forward<Args>(args) + ... +
                      5);  // (1 + (2 + (3 + (4 + 5))))
            return std::make_tuple(a, b, c, d);
        }

    }  // namespace example_2

    namespace example_3 {
        auto print1 = [](auto &&...args) {
            // operator<< 左折叠，std::cout 是初始值
            (std::cout << ... << std::forward<decltype(args)>(args));
        };

        auto print2 = [](auto &&...args) {
            // operator, 左折叠
            ((std::cout << std::forward<decltype(args)>(args) << ","), ...);
        };

    }  // namespace example_3

    namespace example_4 {

        template <typename T>
        void print1(const T &t) {
            std::cout << t << '\n';  // 基本情况
        }

        template <typename T, typename... Args>
        void print1(const T &t, const Args &...args) {
            std::cout << t << ", ";
            print1(args...);  // 递归调用
        }

        template <typename... Args>
        void print2(Args &&...args) {
            (std::cout << ... << args) << '\n';  // 右折叠打印所有参数
        }

        template <typename... Args>
        void print3(Args &&...args) {
            auto dummy = {(std::cout << args << ", ", 0)...};
            (void)dummy;  // 避免未使用变量警告
        }

        template <typename... Args>
        void countArgs(Args &&...args) {
            std::cout << "Number of arguments: " << sizeof...(args) << '\n';
        }

        template <typename... Args>
        void f(const Args &...args) {
            // print(args + 1...);  // ERROR：1... 是带多个小数点的字面值，不合法
            print1(args + 1 ...);   // OK
            print1((args + 1)...);  // OK
        }

        void test() {
            print1(1, 2, 3, 4, 5);     // 1, 2, 3, 4, 5,
            print2(1, 2, 3, 4, 5);     // 12345
            countArgs(1, 2, 3, 4, 5);  // Number of arguments: 5
            f(1, 2, 3, 4, 5);          // 2, 3, 4, 5, 6,
        }
    }  // namespace example_4

    void test() {
        assert(example_1::sum(1, 2, 3, 4, 5) == 15);
        auto [a, b, c, d] = example_2::sum(1, 2, 3, 4);
        example_3::print1(a, b, c, d);  // 10101515
        example_3::print2(a, b, c, d);  // 10,10,15,15,
        example_4::test();
    }
}  // namespace Cpp17FoldExpressionForSum

namespace C17IfConstexpr {
    template <typename T, typename... Args>
    void myPrint(const T &t, Args &&...args) {
        std::cout << t << std::endl;
        if constexpr (sizeof...(args) > 0) {
            myPrint(std::forward<Args>(args)...);
        }
    }

    template <typename T0, typename... T>
    void printf2(T0 t0, T... t) {
        std::cout << t0 << '\n';
        if constexpr (sizeof...(t) > 0) printf2(t...);
    }
}  // namespace C17IfConstexpr

int main() { return 0; }
