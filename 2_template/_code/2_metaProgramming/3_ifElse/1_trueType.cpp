#include <type_traits>

namespace trueTypeUsage {
    template <typename T>
    struct is_pointer : std::false_type {};

    template <typename T>
    struct is_pointer<T*> : std::true_type {};

    static_assert(is_pointer<int*>::value, "T is a pointer");
    static_assert(!is_pointer<int>::value, "T is not a pointer");

}  // namespace trueTypeUsage

namespace trueTypeImpl {
    template <typename T, T v>
    struct integral_constant {
        static constexpr T value = v;
        constexpr operator T() const noexcept { return value; }
    };

    using true_type = integral_constant<bool, true>;
    using false_type = integral_constant<bool, false>;

    template <typename T>
    struct is_pointer : false_type {};

    template <typename T>
    struct is_pointer<T*> : true_type {};

    template <typename T>
    constexpr bool is_pointer_v = is_pointer<T>::value;

    static_assert(is_pointer<int*>::value, "T is a pointer");
    static_assert(!is_pointer<int>::value, "T is not a pointer");
    static_assert(is_pointer_v<int*>, "T is a pointer");
    static_assert(!is_pointer_v<int>, "T is not a pointer");

}  // namespace trueTypeImpl

int main() {}