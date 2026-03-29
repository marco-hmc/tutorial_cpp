#include <iostream>
#include <type_traits>

namespace enableIfUsage {

    template <typename T>
    constexpr typename std::enable_if<std::is_integral<T>::value, T>::type
    conditionalFunction(T value) {
        return value * 2;
    }

    template <typename T>
    constexpr typename std::enable_if<!std::is_integral<T>::value, T>::type
    conditionalFunction(T value) {
        return value + 0.5;
    }

    void test() {
        static_assert(conditionalFunction(10) == 20);
        static_assert(conditionalFunction(10.5) == 11.0);
    }
}  // namespace enableIfUsage

namespace enableIfImpl {

    template <bool B, typename T = void>
    struct enable_if {};

    template <typename T>
    struct enable_if<true, T> {
        using type = T;
    };

    template <typename T>
    constexpr typename enable_if<std::is_integral<T>::value, T>::type
    conditionalFunction(T value) {
        return value * 2;
    }

    template <typename T>
    constexpr typename enable_if<!std::is_integral<T>::value, T>::type
    conditionalFunction(T value) {
        return value + 0.5;
    }

    void test() {
        static_assert(conditionalFunction(10) == 20);
        static_assert(conditionalFunction(10.5) == 11.0);
    }
}  // namespace enableIfImpl

namespace NoNameTypeTemplateParameter {

    template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    void foo(T) {
        // 仅对 integral 可用
    }

    void test() {
        foo(10);  // 有效
        // foo(10.5); // 无效，编译错误
    }

}  // namespace NoNameTypeTemplateParameter

namespace EnableIfInFunctionOverload {

    template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
    void bar(T) {
        // integral 版本
    }

    template <typename T, std::enable_if_t<!std::is_integral_v<T>, int> = 0>
    void bar(T) {
        // 非 integral 版本
    }

    void test() {
        bar(10);    // 调用 integral 版本
        bar(10.5);  // 调用非 integral 版本
    }

}  // namespace EnableIfInFunctionOverload

int main() { return 0; }