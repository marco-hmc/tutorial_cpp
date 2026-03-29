namespace Constexpr_Usage {
    constexpr int factorial(int n) {
        return (n <= 1) ? 1 : n * factorial(n - 1);
    }
    constexpr int value = Constexpr_Usage::factorial(5);  // 编译期计算 5!
    static_assert(value == 120);
}  // namespace Constexpr_Usage

int main() { return 0; }
