#include <cmath>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <stack>
#include <stdexcept>

template <typename IT>
double evaluate_rpn(IT it, IT end) {
    std::stack<double> val_stack;
    auto pop_stack([&]() {
        auto r(val_stack.top());
        val_stack.pop();
        return r;
    });

    std::map<std::string, double (*)(double, double)> ops{
        {"+", [](double a, double b) { return a + b; }},
        {"-", [](double a, double b) { return a - b; }},
        {"*", [](double a, double b) { return a * b; }},
        {"/", [](double a, double b) { return a / b; }},
        {"^", [](double a, double b) { return pow(a, b); }},
        {"%", [](double a, double b) { return fmod(a, b); }},
    };

    for (; it != end; ++it) {
        std::stringstream ss{*it};
        if (double val = NAN; ss >> val) {
            val_stack.push(val);
        } else {
            const auto r{pop_stack()};
            const auto l{pop_stack()};
            try {
                const auto &op(ops.at(*it));
                const double result{op(l, r)};
                val_stack.push(result);
            } catch (const std::out_of_range &) {
                throw std::invalid_argument(*it);
            }
        }
    }

    return val_stack.top();
}

int main() {
    try {
        std::cout << evaluate_rpn(std::istream_iterator<std::string>{std::cin},
                                  {})
                  << '\n';
    } catch (const std::invalid_argument &e) {
        std::cout << "Invalid operator: " << e.what() << '\n';
    }
}