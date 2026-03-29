#include <cstddef>
#include <type_traits>

namespace FactorialCalc {

    template <int N>
    struct Factorial {
        static constexpr int value = N * Factorial<N - 1>::value;
    };

    template <>
    struct Factorial<0> {
        static constexpr int value = 1;
    };

    static_assert(Factorial<5>::value == 120);

}  // namespace FactorialCalc

namespace FibonacciCalc {
    template <std::size_t N>
    struct Fibonacci {
        static constexpr std::size_t value =
            Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
    };

    template <>
    struct Fibonacci<0> {
        static constexpr std::size_t value = 0;
    };
    template <>
    struct Fibonacci<1> {
        static constexpr std::size_t value = 1;
    };

    static_assert(Fibonacci<10>::value == 55);
}  // namespace FibonacciCalc

namespace GCDCalc {
    // Greatest Common Divisor
    template <std::size_t A, std::size_t B>
    struct GCD {
        static constexpr std::size_t value = GCD<B, A % B>::value;
    };

    template <std::size_t A>
    struct GCD<A, 0> {
        static constexpr std::size_t value = A;
    };

    static_assert(GCD<48, 18>::value == 6);
}  // namespace GCDCalc

namespace SqrtCalc {
    template <int N>
    constexpr int sqrt() {
        if constexpr (N <= 1) {
            return N;
        }
        int l = 1;
        int r = N;
        while (l < r) {
            int m = l + (r - l) / 2;
            int t = N / m;
            if (m == t) {
                return m;
            }

            if (m > t) {
                r = m;
            } else {
                l = m + 1;
            }
        }
        return l - 1;
    }

    static_assert(sqrt<10000>() == 100);

    constexpr int square(int x) { return x * x; }
    static_assert(square(5) == 25);
}  // namespace SqrtCalc

namespace SqrtCalc2 {

    template <int N, int L = 1, int R = N>
    struct sqrt {
        static constexpr auto M = L + (R - L) / 2;
        static constexpr auto T = N / M;
        static constexpr auto value =
            std::conditional_t<(T < M), sqrt<N, L, M>, sqrt<N, M + 1, R>>::
                value;  // 避免递归实例化所有分支
    };

    template <int N, int M>
    struct sqrt<N, M, M> {
        static constexpr auto value = M - 1;
    };

    template <int N>
    inline constexpr auto sqrt_v = sqrt<N, 1, N>::value;

    static_assert(sqrt_v<10000> == 100);
}  // namespace SqrtCalc2

int main() { return 0; }