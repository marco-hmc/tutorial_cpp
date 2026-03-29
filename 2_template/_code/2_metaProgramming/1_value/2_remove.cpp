#include <iostream>
#include <type_traits>

namespace removeReferenceUsage {
    template <typename T>
    using remove_reference_t = typename std::remove_reference<T>::type;

    static_assert(std::is_same_v<remove_reference_t<int>, int>,
                  "int 应该是 int");
    static_assert(std::is_same_v<remove_reference_t<int&>, int>,
                  "int& 应该是 int");
    static_assert(std::is_same_v<remove_reference_t<int&&>, int>,
                  "int&& 应该是 int");
}  // namespace removeReferenceUsage

namespace removeReferenceImpl {

    template <typename T>
    struct remove_reference {
        using type = T;
    };

    template <typename T>
    struct remove_reference<T&> {
        using type = T;
    };

    template <typename T>
    struct remove_reference<T&&> {
        using type = T;
    };

    template <typename T>
    using remove_reference_t = typename remove_reference<T>::type;

    template <typename T>
    constexpr bool is_reference_v = std::is_reference<T>::value;

    static_assert(std::is_same_v<remove_reference_t<int>, int>,
                  "int 应该是 int");
    static_assert(std::is_same_v<remove_reference_t<int&>, int>,
                  "int& 应该是 int");
    static_assert(std::is_same_v<remove_reference_t<int&&>, int>,
                  "int&& 应该是 int");
}  // namespace removeReferenceImpl

namespace removeConstUsage {
    template <typename T>
    using remove_const_t = typename std::remove_const<T>::type;

    static_assert(std::is_same_v<remove_const_t<int>, int>, "int 应该是 int");
    static_assert(std::is_same_v<remove_const_t<const int>, int>,
                  "const int 应该是 int");
    // special case
    static_assert(std::is_same_v<remove_const_t<const int&>, const int&>,
                  "const int& 应该是 const int&");
}  // namespace removeConstUsage

namespace removeConstImpl {

    template <typename T>
    struct remove_const {
        using type = T;
    };

    template <typename T>
    struct remove_const<const T> {
        using type = T;
    };

    template <typename T>
    using remove_const_t = typename remove_const<T>::type;

    static_assert(std::is_same_v<remove_const_t<int>, int>, "int 应该是 int");
    static_assert(std::is_same_v<remove_const_t<const int>, int>,
                  "const int 应该是 int");
    static_assert(std::is_same_v<remove_const_t<const int&>, const int&>,
                  "const int& 应该是 const int&");
}  // namespace removeConstImpl

namespace removePointerUsage {
    template <typename T>
    struct RemovePointer {
        using Result = T;
    };

    template <typename T>
    struct RemovePointer<T*> {
        using Result = typename RemovePointer<T>::Result;
    };

    // 静态断言测试
    static_assert(std::is_same_v<RemovePointer<int>::Result, int>,
                  "RemovePointer<int>::Result 应该是 int");
    static_assert(std::is_same_v<RemovePointer<int*>::Result, int>,
                  "RemovePointer<int*>::Result 应该是 int");
    static_assert(std::is_same_v<RemovePointer<int**>::Result, int>,
                  "RemovePointer<int**>::Result 应该是 int");
    static_assert(std::is_same_v<RemovePointer<int***>::Result, int>,
                  "RemovePointer<int***>::Result 应该是 int");

}  // namespace removePointerUsage

////////////////////////////////////////////////////////////////////
namespace removePointerImpl {
    // RemovePointer 的实现
    template <typename T>
    struct RemovePointer {
        using Result = T;
    };

    template <typename T>
    struct RemovePointer<T*> {
        using Result = typename RemovePointer<T>::Result;
    };

    static_assert(std::is_same_v<RemovePointer<int>::Result, int>,
                  "RemovePointer<int>::Result 应该是 int");
    static_assert(std::is_same_v<RemovePointer<int*>::Result, int>,
                  "RemovePointer<int*>::Result 应该是 int");
    static_assert(std::is_same_v<RemovePointer<int**>::Result, int>,
                  "RemovePointer<int**>::Result 应该是 int");
    static_assert(std::is_same_v<RemovePointer<int***>::Result, int>,
                  "RemovePointer<int***>::Result 应该是 int");

}  // namespace removePointerImpl

int main() { return 0; }