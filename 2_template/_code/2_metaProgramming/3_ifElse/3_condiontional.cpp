#include <type_traits>

namespace conditionalUsage {
    struct TrueType {
        static constexpr const char* name = "TrueType";
    };

    struct FalseType {
        static constexpr const char* name = "FalseType";
    };

    using SelectedType1 = std::conditional<true, TrueType, FalseType>::type;
    static_assert(std::is_same_v<SelectedType1, TrueType>,
                  "SelectedType1 应该是 TrueType");

    using SelectedType2 = std::conditional<false, TrueType, FalseType>::type;
    static_assert(std::is_same_v<SelectedType2, FalseType>,
                  "SelectedType2 应该是 FalseType");

}  // namespace conditionalUsage

namespace conditionalImpl {

    template <bool B, typename T, typename F>
    struct conditional {
        using type = T;
    };

    template <typename T, typename F>
    struct conditional<false, T, F> {
        using type = F;
    };

    struct TrueType {
        static constexpr const char* name = "TrueType";
    };

    struct FalseType {
        static constexpr const char* name = "FalseType";
    };

    using SelectedType1 = conditional<true, TrueType, FalseType>::type;
    static_assert(std::is_same_v<SelectedType1, TrueType>,
                  "SelectedType1 应该是 TrueType");

    using SelectedType2 = conditional<false, TrueType, FalseType>::type;
    static_assert(std::is_same_v<SelectedType2, FalseType>,
                  "SelectedType2 应该是 FalseType");
}  // namespace conditionalImpl

int main() { return 0; }