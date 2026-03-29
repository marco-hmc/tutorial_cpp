#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

template <typename T>
auto map(T fn) {
    return [=](auto reduce_fn) {
        return
            [=](auto accum, auto input) { return reduce_fn(accum, fn(input)); };
    };
}

template <typename T>
auto filter(T predicate) {
    return [=](auto reduce_fn) {
        return [=](auto accum, auto input) {
            if (predicate(input)) {
                return reduce_fn(accum, input);
            }
            return accum;
        };
    };
}

int main() {
    std::vector<int> input_data{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto even([](int i) { return i % 2 == 0; });
    auto twice([](int i) { return i * 2; });
    auto copy_and_advance([](auto it, auto input) {
        *it = input;
        return ++it;
    });

    std::vector<int> output_data;
    std::accumulate(input_data.begin(), input_data.end(),
                    std::back_inserter(output_data),
                    filter(even)(map(twice)(copy_and_advance)));

    for (const auto& val : output_data) {
        std::cout << val << ", ";
    }
    std::cout << '\n';

    return 0;
}
