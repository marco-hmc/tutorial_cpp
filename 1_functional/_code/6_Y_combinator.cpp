#include <functional>
#include <iostream>

/*
1. **什么是Y combinator?**
    Y-combinator是一个高阶函数，用于实现递归。它允许你在没有显式命名函数的情况下定义递归函数。

2. **为什么需要Y combinator?**
   在某些编程环境中，函数不能直接引用自身。Y-combinator提供了一种方法，使得即使在这种限制下，也可以定义递归函数。

3. **为什么Y combinator可以递归调用自己?**
   Y-combinator利用了函数的自应用，通过将一个函数应用于自身，能够在没有显式命名的情况下实现递归。通过传递一个函数作为参数，
   Y-combinator可以递归地调用自身。

4.
    对于现代 C++ 来说，通常不需要通过 Y-combinator 的方式来实现递归。现代 C++
    提供了足够的语言特性，如 lambda 表达式、自动类型推断（auto
    关键字）、模板等，这些特性使得在不使用 Y-combinator
    的情况下实现递归变得简单且直观。在实际的 C++
    应用开发中，直接使用函数名或适当的函数对象来实现递归通常更为常见和推荐。

    Y-combinator
    在现代C++中的主要意义确实更多地体现在教育价值上，它帮助开发者理解以下概念：
        递归的本质：Y-combinator展示了递归可以在没有命名函数的情况下实现，这有助于理解递归调用的核心原理。
        高阶函数：通过
    Y-combinator的学习，开发者可以更好地理解高阶函数的概念，即函数可以接受函数作为参数或返回函数作为结果。
        匿名函数和闭包：Y-combinator 的实现通常涉及匿名函数（如
    lambda表达式）和闭包，这有助于开发者理解这些概念及其在 C++ 中的应用。
    总之，虽然在现代 C++
    开发中直接使用Y-combinator的场景不多，但作为一个理论概念和学习工具，Y-combinator
        对于深入理解函数式编程思想、递归以及高阶函数等概念非常有价值。
*/

namespace FactorialByYCombinator {
    template <typename F>
    struct Y {
        F f;
        template <typename... Args>
        auto operator()(Args &&...args) const {
            return f(*this, std::forward<Args>(args)...);
        }
    };

    template <typename F>
    Y<F> make_y_combinator(F &&f) {
        return {std::forward<F>(f)};
    }

    void test_y_combinator() {
        auto factorial = make_y_combinator([](auto self, int n) -> int {
            if (n <= 1) {
                return 1;
            }
            return n * self(n - 1);
        });
        std::cout << "Factorial of 5: " << factorial(5) << '\n';
    }
}  // namespace FactorialByYCombinator

// Factorial function using Y combinator
////////////////////////////////////////////////////////////////////////////////////////

namespace FactorialByY {
    auto Y = [](auto f) {
        return [f](auto x) -> std::function<int(int)> {
            return f([x](auto v) { return x(x)(v); });
        }([f](auto x) -> std::function<int(int)> {
                   return f([x](auto v) { return x(x)(v); });
               });
    };

    auto factorial = Y([](auto f) {
        return [f](int n) { return (n == 0) ? 1 : n * f(n - 1); };
    });

    void test_y_combinator() {
        std::cout << "Factorial of 5: " << factorial(5) << '\n';
    }
}  // namespace FactorialByY

int main() {
    FactorialByYCombinator ::test_y_combinator();
    FactorialByY::test_y_combinator();
    return 0;
}
