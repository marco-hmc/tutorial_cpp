#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>

namespace time_literal {

    class Time {
      public:
        explicit Time(int64_t seconds) : seconds_(seconds) {}

        int64_t getSeconds() const { return seconds_; }
        int64_t getMinutes() const { return seconds_ / 60; }
        int64_t getHours() const { return seconds_ / 3600; }

      private:
        int64_t seconds_;
    };

    Time operator"" _sec(unsigned long long seconds) { return Time(seconds); }
    Time operator"" _min(unsigned long long minutes) {
        return Time(minutes * 60);
    }
    Time operator"" _hr(unsigned long long hours) { return Time(hours * 3600); }

    void task() {
        Time t1 = 120_sec;
        Time t2 = 2_min;
        Time t3 = 1_hr;

        std::cout << "t1: " << t1.getSeconds() << " seconds" << std::endl;
        std::cout << "t2: " << t2.getMinutes() << " minutes" << std::endl;
        std::cout << "t3: " << t3.getHours() << " hours" << std::endl;
    }
}  // namespace time_literal

namespace string_literal {
    // 0b表示二进制
    // 0表示八进制
    // 0x表示十六进制

    // 有些是不区分大小写的
    // u表示unsigned
    // l表示long
    // ll表示long long
    const unsigned int x = 1U;
    const std::uint64_t y = 1UL;
    const uint64_t z = 1LU;
    const unsigned long long w = 1ULL;
    const unsigned long long v = 1LLU;
    const long a = 1l;
    const long b = 1L;
    const long long c = 1ll;
    const long long d = 1LL;

    // u8表示UTF-8字符串
    // u表示UTF-16字符串
    // U表示UTF-32字符串
    // L表示宽字符串
    const char8_t *utf8Str = u8"这是一个UTF-8字符串";
    const char16_t *utf16Str = u"这是一个UTF-16字符串";
    const char32_t *utf32Str = U"这是一个UTF-32字符串";
    const wchar_t *wideStr = L"这是一个宽字符串";

    long long operator"" _celsius(unsigned long long tempCelsius) {
        return std::llround(tempCelsius * 1.8 + 32);
    }
    int operator"" _int(const char *str) { return std::stoi(str); }

    void task() {
        std::cout << "36_celsius: " << 36_celsius << " Fahrenheit" << std::endl;
        std::cout << "123_int: " << 123_int << std::endl;
    }
}  // namespace string_literal

int main() { return 0; }