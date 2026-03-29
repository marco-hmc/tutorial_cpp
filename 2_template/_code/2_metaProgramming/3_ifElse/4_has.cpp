#include <type_traits>
#include <utility>
#include <vector>

namespace void_tUsage {

    /*
    1. std::void_t 怎么用？
    std::void_t 是一个用于 SFINAE（Substitution Failure Is Not An Error）的工具。
    std::void_t 是一个可变参数模板别名，它接受任意数量和类型的模板参数，但无论传入什么模板参数，它都会将这些参数 “忽略”，最终的结果类型总是 void。

    它常用于编译时检查一个类型是否具有某个特定的成员类型、成员函数或属性。
    如果替换失败，编译器不会报错，而是选择其他重载或模板特化。
    */

    template <typename, typename = std::void_t<>>
    struct has_type : std::false_type {};

    template <typename T>
    struct has_type<T, std::void_t<typename T::type>> : std::true_type {};

    template <typename T>
    inline constexpr bool has_type_v = has_type<T>::value;

    struct A {
        using type = int;
    };

    struct B {};

    static_assert(has_type_v<A>, "A has a member named type");
    static_assert(!has_type_v<B>, "B does not have a member named type");

}  // namespace void_tUsage

////////////////////////////////////////////////////////////////////
namespace void_tImpl {
    template <typename... Ts>
    using void_t = void;

    template <typename, typename = void_t<>>
    struct has_type : std::false_type {};

    template <typename T>
    struct has_type<T, void_t<typename T::type>> : std::true_type {};

    template <typename T>
    inline constexpr bool has_type_v = has_type<T>::value;

    struct A {
        using type = int;
    };

    struct B {};

    static_assert(has_type_v<A>, "A has a member named type");
    static_assert(!has_type_v<B>, "B does not have a member named type");

}  // namespace void_tImpl

namespace hasConstXGetterUsage {
    /*
1. has_const_x_getter 怎么用？
    has_const_x_getter 是一个模板元编程工具，用于在编译时检查类型是否具有一个 const 成员函数 x。
    它常用于 SFINAE（Substitution Failure Is Not An Error）技术中，根据条件选择合适的模板实例化。
*/

    // has_const_x_getter 的实现
    template <typename T>
    struct has_const_x_getter {
      private:
        static int detect(...);

        template <typename V>
        static auto detect(const V& v) -> decltype((int)(v.x()), void()) {}

      public:
        static constexpr bool value =
            std::is_void_v<decltype(detect(std::declval<T>()))>;
    };

    template <typename T>
    constexpr bool has_const_x_getter_v = has_const_x_getter<T>::value;

    struct A {
        int x() const;
    };
    static_assert(has_const_x_getter_v<A>);

    struct B {
        int x();
    };
    static_assert(!has_const_x_getter_v<B>);

    struct C {
        void x() const;
    };
    static_assert(!has_const_x_getter_v<C>);

    struct D {
        int y() const;
    };
    static_assert(!has_const_x_getter_v<D>);

    struct E {};
    static_assert(!has_const_x_getter_v<E>);

    struct F {
        const int& x() const;
    };
    static_assert(has_const_x_getter_v<F>);

}  // namespace hasConstXGetterUsage

////////////////////////////////////////////////////////////////////
namespace hasConstXGetterImpl {
    // has_const_x_getter 的实现
    template <typename T>
    struct has_const_x_getter {
      private:
        static int detect(...);

        template <typename V>
        static auto detect(const V& v) -> decltype((int)(v.x()), void()) {}

      public:
        static constexpr bool value =
            std::is_void_v<decltype(detect(std::declval<T>()))>;
    };

    template <typename T>
    constexpr bool has_const_x_getter_v = has_const_x_getter<T>::value;

    struct A {
        int x() const;
    };
    static_assert(has_const_x_getter_v<A>);

    struct B {
        int x();
    };
    static_assert(!has_const_x_getter_v<B>);

    struct C {
        void x() const;
    };
    static_assert(!has_const_x_getter_v<C>);

    struct D {
        int y() const;
    };
    static_assert(!has_const_x_getter_v<D>);

    struct E {};
    static_assert(!has_const_x_getter_v<E>);

    struct F {
        const int& x() const;
    };
    static_assert(has_const_x_getter_v<F>);

}  // namespace hasConstXGetterImpl

namespace detectionIdiomUsage {
    /*
    1. 检测习惯用法 (Detection idiom)
        检测习惯用法是一种模板元编程技术，用于在编译时检查类型是否具有特定的成员函数或成员类型。
        它常用于 SFINAE（Substitution Failure Is Not An Error）技术中，根据条件选择合适的模板实例化。
    */

    template <typename, template <typename...> class Op, typename... Args>
    struct detector : std::false_type {};

    template <template <typename...> class Op, typename... Args>
    struct detector<std::void_t<Op<Args...>>, Op, Args...> : std::true_type {};

    template <template <typename...> class Op, typename... Args>
    using is_detected = detector<void, Op, Args...>;

    template <typename T>
    using has_emplace_back = decltype(std::declval<T>().emplace_back(
        std::declval<typename T::value_type>()));

    template <typename T>
    constexpr bool has_emplace_back_v =
        is_detected<has_emplace_back, std::remove_reference_t<T>>::value;

    static_assert(has_emplace_back_v<std::vector<int>>,
                  "std::vector<int> 应该具有 emplace_back 成员函数");
    static_assert(has_emplace_back_v<std::vector<int>&>,
                  "std::vector<int>& 应该具有 emplace_back 成员函数");
    static_assert(has_emplace_back_v<std::vector<int>&&>,
                  "std::vector<int>&& 应该具有 emplace_back 成员函数");
}  // namespace detectionIdiomUsage

////////////////////////////////////////////////////////////////////
namespace detectionIdiomImpl {
    template <typename, template <typename...> class Op, typename... Args>
    struct detector : std::false_type {};

    template <template <typename...> class Op, typename... Args>
    struct detector<std::void_t<Op<Args...>>, Op, Args...> : std::true_type {};

    template <template <typename...> class Op, typename... Args>
    using is_detected = detector<void, Op, Args...>;

    template <typename T>
    using has_emplace_back = decltype(std::declval<T>().emplace_back(
        std::declval<typename T::value_type>()));

    template <typename T>
    constexpr bool has_emplace_back_v =
        is_detected<has_emplace_back, std::remove_reference_t<T>>::value;

    static_assert(has_emplace_back_v<std::vector<int>>,
                  "std::vector<int> 应该具有 emplace_back 成员函数");
    static_assert(has_emplace_back_v<std::vector<int>&>,
                  "std::vector<int>& 应该具有 emplace_back 成员函数");
    static_assert(has_emplace_back_v<std::vector<int>&&>,
                  "std::vector<int>&& 应该具有 emplace_back 成员函数");

}  // namespace detectionIdiomImpl

int main() { return 0; }
