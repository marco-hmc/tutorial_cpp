#include <cstddef>

namespace Template_Ptr_Infer {

    template <typename T>
    struct A;

    template <typename T, std::size_t N>
    struct A<T[N]> {
        static constexpr int value = 1;
    };

    template <typename T, std::size_t N>
    struct A<T (&)[N]> {
        static constexpr int value = 2;
    };

    template <typename T>
    struct A<T[]> {
        static constexpr int value = 3;
    };

    template <typename T>
    struct A<T (&)[]> {
        static constexpr int value = 4;
    };

    template <typename T>
    struct A<T*> {
        static constexpr int value = 5;
    };

    int x[] = {1, 2, 3, 4, 5};

    template <typename T1, typename T2, typename T3>
    constexpr void task(int a1[7], int a2[], int (&a3)[42], int (&x0)[], T1 x1,
                        T2& x2, T3&& x3) {
        static_assert(A<decltype(a1)>::value == 5);  // A<T*>
        static_assert(A<decltype(a2)>::value == 5);  // A<T*>
        static_assert(A<decltype(a3)>::value == 2);  // A<T(&)[N]>
        static_assert(A<decltype(x0)>::value == 4);  // A<T(&)[]>
        static_assert(A<decltype(x1)>::value == 5);  // A<T*>
        static_assert(A<decltype(x2)>::value == 4);  // A<T(&)[]>
        static_assert(A<decltype(x3)>::value == 4);  // A<T(&)[]>
    }

    void task() {
        int a[42];
        static_assert(Template_Ptr_Infer::A<decltype(a)>::value == 1);
        extern int x[];
        static_assert(Template_Ptr_Infer::A<decltype(x)>::value == 3);

        Template_Ptr_Infer::task(a, a, a, x, x, x, x);
    }
}  // namespace Template_Ptr_Infer

int main() {
    Template_Ptr_Infer::task();
    return 0;
}