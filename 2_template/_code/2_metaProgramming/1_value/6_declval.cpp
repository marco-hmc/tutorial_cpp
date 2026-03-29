#include <iostream>
#include <type_traits>

/*
1. std::declval 怎么用？
    std::declval 是一个在编译时用于类型推断的工具，它可以用来获取一个类型的引用，而不需要实际构造该类型的对象。
    这在处理无法默认构造或不想构造对象的类型时特别有用。
*/

namespace declvalUsage {

    class MyClass {
      public:
        int myFunction() const { return 42; }
    };

    template <typename T>
    auto testFunctionReturnType() -> decltype(std::declval<T>().myFunction()) {
        return std::declval<T>().myFunction();
    }

    static_assert(
        std::is_same<decltype(testFunctionReturnType<MyClass>()), int>::value,
        "返回类型应为 int");

}  // namespace declvalUsage

namespace declvalImpl {

    template <class T>
    std::add_rvalue_reference_t<T> declval() noexcept;

    class MyClass {
      public:
        int myFunction() const { return 42; }
    };

    template <typename T>
    auto testFunctionReturnType() -> decltype(declval<T>().myFunction()) {
        return declval<T>().myFunction();
    }

    static_assert(
        std::is_same<decltype(testFunctionReturnType<MyClass>()), int>::value,
        "返回类型应为 int");

}  // namespace declvalImpl

int main() { return 0; }