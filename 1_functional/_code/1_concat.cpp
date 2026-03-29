#include <functional>
#include <future>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

namespace concat_case1 {

    template <typename T, typename... Ts>
    auto concat(T t, Ts... ts) {
        if constexpr (sizeof...(ts) > 0) {
            return [=](auto... parameters) {
                return t(concat(ts...)(parameters...));
            };
        } else {
            return [=](auto... parameters) { return t(parameters...); };
        }
    }

    void task() {
        auto twice = [](int i) { return i * 2; };
        auto thrice = [](int i) { return i * 3; };
        auto combined = concat(twice, thrice, std::plus<int>{});

        std::cout << combined(2, 3) << '\n';
    }
}  // namespace concat_case1

namespace concat_case2 {
    using namespace std::chrono_literals;

    struct pcout : public std::stringstream {
        static inline std::mutex cout_mutex;

        ~pcout() override {
            std::lock_guard<std::mutex> l{cout_mutex};
            std::cout << rdbuf();
            std::cout.flush();
        }
    };

    std::string create(const char *s) {
        pcout{} << "3s CREATE " << std::quoted(s) << '\n';
        std::this_thread::sleep_for(3s);
        return {s};
    }

    std::string concat(const std::string &a, const std::string &b) {
        pcout{} << "5s CONCAT " << std::quoted(a) << " " << std::quoted(b)
                << '\n';
        std::this_thread::sleep_for(5s);
        return a + b;
    }

    std::string twice(const std::string &s) {
        pcout{} << "3s TWICE " << std::quoted(s) << '\n';
        std::this_thread::sleep_for(3s);
        return s + s;
    }

    template <typename F>
    auto asynchronous(F f) {
        return [f](auto... xs) {
            return [=]() { return std::async(std::launch::async, f, xs...); };
        };
    }

    template <typename F>
    auto fut_unwrap(F f) {
        return [f](auto... xs) { return f(xs.get()...); };
    }

    template <typename F>
    auto async_adapter(F f) {
        return [f](auto... xs) {
            return [=]() {
                return std::async(std::launch::async, fut_unwrap(f), xs()...);
            };
        };
    }

    void task() {
        auto pCreate = asynchronous(create);
        auto pConcat = async_adapter(concat);
        auto pTwice = async_adapter(twice);
        auto result = pConcat(pTwice(pConcat(pCreate("foo "), pCreate("bar "))),
                              pConcat(pCreate("this "), pCreate("that ")));

        std::cout << "Setup done. Nothing executed yet.\n";

        std::cout << result().get() << '\n';
    }
}  // namespace concat_case2

int main() { return 0; }