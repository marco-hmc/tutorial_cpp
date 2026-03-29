#include <stdarg.h>

#include <cassert>
#include <iostream>
#include <tuple>

namespace PrintAll {
    template <std::size_t Index, std::size_t N, typename... List>
    struct PrintImpl {
        static void impl(const std::tuple<List...>& t) {
            std::cout << std::get<Index>(t) << " ";
            PrintImpl<Index + 1, N, List...>::impl(t);
        }
    };

    template <std::size_t N, typename... List>
    struct PrintImpl<N, N, List...> {
        static void impl(const std::tuple<List...>& t) {}
    };

    template <typename... List>
    void Print(const std::tuple<List...>& t) {
        PrintImpl<0, sizeof...(List), List...>::impl(t);
    }

    void test() {
        auto t = std::make_tuple(3.14, 42, "hello world");
        Print(t);  // 输出: 3.14 42 hello world
        std::cout << std::endl;
    }
}  // namespace PrintAll

namespace TemplateCalcMax {

    template <int N, int... Ns>
    struct max;

    template <int N>
    struct max<N> : std::integral_constant<int, N> {};

    template <int N1, int N2, int... Ns>
    struct max<N1, N2, Ns...>
        : std::integral_constant<int, (N1 < N2) ? max<N2, Ns...>::value
                                                : max<N1, Ns...>::value> {};

    template <int... Ns>
    inline constexpr auto max_v = max<Ns...>::value;

    void test() { static_assert(TemplateCalcMax::max_v<3, 2, 1, 5, 4> == 5); }

}  // namespace TemplateCalcMax

int main() { return 0; }
