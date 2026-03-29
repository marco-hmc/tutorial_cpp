#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

/*
    1. std::is_convertible 怎么用？
        std::is_convertible 用于检查一个类型是否可以转换为另一个类型。
        如果类型 From 可以隐式转换为类型 To，则 std::is_convertible<From, To>::value 为 true；
        否则为 false。

    2. std::is_same 怎么用？
        std::is_same 用于检查两个类型是否相同。
        如果类型 T 和 U 相同，则 std::is_same<T, U>::value 为 true；
        否则为 false。
*/

namespace is_base_ofUsage {
    /*
    1. std::is_base_of 怎么用？
    std::is_base_of 用于检查一个类型是否是另一个类型的基类。
    如果 Base 是 Derived 的基类或相同类型，则 std::is_base_of<Base, Derived>::value 为 true；
    否则为 false。
*/
    struct GrandPa {};
    struct Parent : GrandPa {};
    struct Son : Parent {};
    struct Unrelated {};

    static_assert(std::is_base_of<GrandPa, Parent>::value, "wrong");
    static_assert(!std::is_base_of<Parent, GrandPa>::value, "wrong");
    static_assert(!std::is_base_of<GrandPa, Unrelated>::value, "wrong");
    static_assert(std::is_base_of<GrandPa, Son>::value, "wrong");
    static_assert(!std::is_base_of<Unrelated, Son>::value, "wrong");

}  // namespace is_base_ofUsage

namespace is_base_ofImpl {
    // 这个实现是错误的，因为它只检查了 Derived 是否可以转换为 Base，而没有检查 Base 是否是 Derived 的基类。
    // 换句话说就是判断两个能不能进行static_cast进行互相转换，而不是判断继承关系。
    // 这个实现只是为了演示 SFINAE 技术，实际上不应该这样使用。
    // std::is_base_of的实现依赖于编译器的内部实现，不应该自己实现。
    template <bool B>
    struct bool_constant {
        static constexpr bool value = B;
    };

    using true_type = bool_constant<true>;
    using false_type = bool_constant<false>;

    template <typename Base, typename Derived>
    auto test(int)
        -> decltype(static_cast<Base*>(static_cast<Derived*>(nullptr)),
                    true_type{});

    template <typename, typename>
    false_type test(...);

    template <typename Base, typename Derived>
    struct is_base_of : decltype(test<Base, Derived>(0)) {};

    class Base {};
    class Derived : public Base {};
    class Unrelated {};

    void task() {
        static_assert(is_base_of<Base, Derived>::value,
                      "Base should be a base of Derived");
        static_assert(!is_base_of<Base, Unrelated>::value,
                      "Base should not be a base of Unrelated");
        static_assert(is_base_of<Derived, Base>::value,
                      "Base should be a base of Derived");
    }

}  // namespace is_base_ofImpl

namespace is_convertibleUsage {
    struct A {};
    struct B : A {};
    struct C {};

    static_assert(std::is_convertible<B, A>::value, "B 应该可以转换为 A");
    static_assert(!std::is_convertible<A, B>::value, "A 不应该可以转换为 B");
    static_assert(!std::is_convertible<C, A>::value, "C 不应该可以转换为 A");
}  // namespace is_convertibleUsage

namespace is_convertibleImpl {
    template <typename T>
    struct is_void : std::is_same<void, typename std::remove_cv<T>::type> {};

    template <typename From, typename To,
              bool = is_void<From>::value || is_void<To>::value>
    struct is_convertible_helper : std::false_type {};

    template <typename From, typename To>
    struct is_convertible_helper<From, To, false> {
      private:
        static void test(To);
        template <typename F, typename = decltype(test(std::declval<F>()))>
        static std::true_type test_convertible(int);
        template <typename>
        static std::false_type test_convertible(...);

      public:
        static constexpr bool value =
            decltype(test_convertible<From>(0))::value;
    };

    template <typename From, typename To>
    struct is_convertible : is_convertible_helper<From, To> {};
}  // namespace is_convertibleImpl

namespace is_sameUsage {

    static_assert(std::is_same<int, int>::value, "int 应该与 int 相同");
    static_assert(!std::is_same<int, double>::value,
                  "int 不应该与 double 相同");
}  // namespace is_sameUsage

namespace is_sameImpl {
    template <typename T, typename U>
    struct is_same {
        static constexpr bool value = false;
    };

    template <typename T>
    struct is_same<T, T> {
        static constexpr bool value = true;
    };

    template <typename T, typename U>
    inline constexpr bool is_same_v = is_same<T, U>::value;
}  // namespace is_sameImpl

namespace isConstructibleUsage {
    /*
2. std::is_constructible 怎么用？
    std::is_constructible 是一个模板，用于检查一个类型是否可以使用给定的参数进行构造。
    它可以用于模板元编程中，作为类型特性的检查工具。
*/
    template <typename T, typename... Args>
    using is_constructible_t = typename std::is_constructible<T, Args...>::type;

    static_assert(std::is_constructible<int>::value, "int 应该是可默认构造的");
    static_assert(std::is_constructible<int, int>::value,
                  "int 应该是可用 int 构造的");
    static_assert(!std::is_constructible<int, double*>::value,
                  "int 不应该是可用 double* 构造的");

    struct A {
        A(int) {}
    };

    static_assert(std::is_constructible<A, int>::value,
                  "A 应该是可用 int 构造的");
    static_assert(!std::is_constructible<A, double*>::value,
                  "A 不应该是可用 double* 构造的");
    static_assert(std::is_constructible<std::string, const char*>::value);
    static_assert(!std::is_constructible<std::string, int>::value,
                  "std::string 不应该是可用 int 构造的");
    static_assert(
        std::is_constructible<std::vector<int>,
                              std::initializer_list<int>>::value,
        "std::vector<int> 应该是可用 std::initializer_list<int> 构造的");
    static_assert(std::is_constructible<std::vector<int>, int>::value,
                  "std::vector<int> 不应该是可用 int 构造的");

}  // namespace isConstructibleUsage

namespace isConstructibleImpl {

    template <typename T, typename... Args>
    struct is_constructible {
      private:
        template <typename U, typename = decltype(U(std::declval<Args>()...))>
        static std::true_type test(void*);

        template <typename>
        static std::false_type test(...);

      public:
        static constexpr bool value = decltype(test<T>(nullptr))::value;
    };

    template <typename T, typename... Args>
    constexpr bool is_constructible_v = is_constructible<T, Args...>::value;

    static_assert(is_constructible_v<int>, "int 应该是可默认构造的");
    static_assert(is_constructible_v<int, int>, "int 应该是可用 int 构造的");
    static_assert(!is_constructible_v<int, double*>,
                  "int 不应该是可用 double* 构造的");

    struct A {
        A(int) {}
    };

    static_assert(is_constructible_v<A, int>, "A 应该是可用 int 构造的");
    static_assert(!is_constructible_v<A, double*>,
                  "A 不应该是可用 double* 构造的");

}  // namespace isConstructibleImpl

int main() { return 0; }
