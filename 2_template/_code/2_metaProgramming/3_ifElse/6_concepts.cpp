#include <algorithm>
#include <concepts>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <type_traits>
#include <vector>

/*
====================================================================================================
                                C++20 Concepts 教学文档
====================================================================================================

1. 什么是Concepts？
   - Concepts是C++20引入的特性，用于对模板参数进行约束
   - 提供了更清晰的模板接口和更好的错误信息
   - 替代了SFINAE技术，使代码更易读和维护

2. Concepts的优势：
   - 编译时约束检查
   - 更清晰的错误信息
   - 自文档化的代码
   - 更好的重载解析

3. 定义Concepts的方式：
   - 使用requires表达式
   - 基于已有的type traits
   - 组合多个概念
   - 嵌套约束

4. 使用场景：
   - 模板参数约束
   - 函数重载选择
   - 条件编译
   - 类型特化

====================================================================================================
*/

namespace BasicConcepts {

    // 基础概念：检查类型是否可迭代
    template <typename T>
    concept Iterable = requires(T t) {
        t.begin();
        t.end();
    };

    // 基础概念：检查类型是否支持算术运算
    template <typename T>
    concept Arithmetic = std::is_arithmetic_v<T>;

    // 基础概念：检查类型是否可比较
    template <typename T>
    concept Comparable = requires(T a, T b) {
        { a == b } -> std::convertible_to<bool>;
        { a != b } -> std::convertible_to<bool>;
        { a < b } -> std::convertible_to<bool>;
        { a > b } -> std::convertible_to<bool>;
    };

    // 基础概念：检查类型是否可打印
    template <typename T>
    concept Printable = requires(T t, std::ostream& os) {
        { os << t } -> std::convertible_to<std::ostream&>;
    };

    void demonstrateBasicConcepts() {
        std::cout << "\n=== 基础概念演示 ===" << std::endl;

        std::vector<int> vec{1, 2, 3};
        std::string str = "hello";
        int num = 42;

        std::cout << "std::vector<int> 是否可迭代: "
                  << Iterable<std::vector<int>> << std::endl;
        std::cout << "std::string 是否可迭代: "
                  << Iterable<std::string> << std::endl;
        std::cout << "int 是否可迭代: " << Iterable<int> << std::endl;

        std::cout << "int 是否为算术类型: " << Arithmetic<int> << std::endl;
        std::cout << "double 是否为算术类型: "
                  << Arithmetic<double> << std::endl;
        std::cout << "std::string 是否为算术类型: "
                  << Arithmetic<std::string> << std::endl;

        std::cout << "int 是否可比较: " << Comparable<int> << std::endl;
        std::cout << "std::string 是否可比较: "
                  << Comparable<std::string> << std::endl;
    }

}  // namespace BasicConcepts

namespace ConceptConstrainedFunctions {

    using namespace BasicConcepts;

    // 使用概念约束的函数模板
    template <Iterable T>
    void print_container(const T& container) {
        std::cout << "容器内容: ";
        for (const auto& item : container) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    template <Arithmetic T>
    void print_number(T value) {
        std::cout << "数字: " << value << std::endl;
    }

    // 重载：优先选择可迭代的类型
    template <typename T>
    void print_value(const T& value)
        requires Iterable<T>
    {
        std::cout << "打印容器: ";
        print_container(value);
    }

    template <typename T>
    void print_value(const T& value)
        requires(!Iterable<T>)
    {
        std::cout << "打印单值: " << value << std::endl;
    }

    void demonstrateConstrainedFunctions() {
        std::cout << "\n=== 概念约束函数演示 ===" << std::endl;

        std::vector<int> vec{1, 2, 3, 4, 5};
        std::string str = "Hello";
        int num = 42;
        double d = 3.14;

        print_container(vec);
        print_container(str);

        print_number(num);
        print_number(d);

        print_value(vec);  // 调用容器版本
        print_value(num);  // 调用单值版本
    }

}  // namespace ConceptConstrainedFunctions

namespace AdvancedConcepts {

    // 高级概念：容器概念
    template <typename T>
    concept Container = requires(T t) {
        typename T::value_type;
        typename T::iterator;
        t.begin();
        t.end();
        t.size();
        t.empty();
    };

    // 高级概念：可插入容器
    template <typename T>
    concept InsertableContainer =
        Container<T> && requires(T t, typename T::value_type v) {
            t.insert(t.end(), v);
        } || requires(T t, typename T::value_type v) { t.push_back(v); };

    // 高级概念：支持随机访问的容器
    template <typename T>
    concept RandomAccessContainer = Container<T> && requires(T t, size_t i) {
        { t[i] } -> std::convertible_to<typename T::value_type>;
        t.at(i);
    };

    // 概念组合：可排序的容器
    template <typename T>
    concept SortableContainer =
        Container<T> && std::random_access_iterator<typename T::iterator> &&
        BasicConcepts::Comparable<typename T::value_type>;

    // 使用高级概念的函数
    template <Container T>
    void analyze_container(const T& container) {
        std::cout << "容器分析:" << std::endl;
        std::cout << "  大小: " << container.size() << std::endl;
        std::cout << "  是否为空: " << (container.empty() ? "是" : "否")
                  << std::endl;
        std::cout << "  支持随机访问: "
                  << (RandomAccessContainer<T> ? "是" : "否") << std::endl;
        std::cout << "  可排序: " << (SortableContainer<T> ? "是" : "否")
                  << std::endl;
    }

    template <InsertableContainer T>
    void add_elements(T& container,
                      const std::vector<typename T::value_type>& elements) {
        std::cout << "向容器添加元素..." << std::endl;

        if constexpr (requires(T t, typename T::value_type v) {
                          t.push_back(v);
                      }) {
            // 使用 push_back
            for (const auto& elem : elements) {
                container.push_back(elem);
            }
        } else {
            // 使用 insert
            for (const auto& elem : elements) {
                container.insert(container.end(), elem);
            }
        }
    }

    template <SortableContainer T>
    void sort_and_print(T& container) {
        std::sort(container.begin(), container.end());
        std::cout << "排序后的容器: ";
        for (const auto& item : container) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    void demonstrateAdvancedConcepts() {
        std::cout << "\n=== 高级概念演示 ===" << std::endl;

        std::vector<int> vec{3, 1, 4, 1, 5};
        std::string str = "hello";

        analyze_container(vec);
        analyze_container(str);

        std::vector<int> new_elements{9, 2, 6};
        add_elements(vec, new_elements);

        sort_and_print(vec);
    }

}  // namespace AdvancedConcepts

namespace ConceptUtilities {

    // 概念检测工具
    template <typename T, template <typename> class Concept>
    constexpr bool satisfies_concept_v = Concept<T>::value;

    // 概念选择器
    template <typename T>
    using element_type = std::conditional_t<AdvancedConcepts::Container<T>,
                                            typename T::value_type, T>;

    // 条件概念
    template <typename T, typename U>
    concept SameAs = std::same_as<T, U>;

    template <typename T, typename U>
    concept ConvertibleTo = std::convertible_to<T, U>;

    // 复合概念
    template <typename T>
    concept NumericContainer =
        AdvancedConcepts::Container<T> &&
        BasicConcepts::Arithmetic<typename T::value_type>;

    // 概念特化的算法
    template <NumericContainer T>
    auto sum_elements(const T& container) {
        using ValueType = typename T::value_type;
        return std::accumulate(container.begin(), container.end(), ValueType{});
    }

    template <BasicConcepts::Arithmetic T>
    auto sum_elements(T value) {
        return value;
    }

    // 概念约束的类模板
    template <typename T>
        requires BasicConcepts::Comparable<T>
    class MinMaxTracker {
      private:
        T min_val;
        T max_val;
        bool initialized;

      public:
        MinMaxTracker() : initialized(false) {}

        void update(const T& value) {
            if (!initialized) {
                min_val = max_val = value;
                initialized = true;
            } else {
                if (value < min_val) min_val = value;
                if (value > max_val) max_val = value;
            }
        }

        T get_min() const { return min_val; }
        T get_max() const { return max_val; }
    };

    void demonstrateUtilities() {
        std::cout << "\n=== 概念工具演示 ===" << std::endl;

        std::vector<int> int_vec{1, 2, 3, 4, 5};
        std::vector<double> double_vec{1.1, 2.2, 3.3};

        std::cout << "整数容器求和: " << sum_elements(int_vec) << std::endl;
        std::cout << "浮点容器求和: " << sum_elements(double_vec) << std::endl;
        std::cout << "单个数字: " << sum_elements(42) << std::endl;

        MinMaxTracker<int> tracker;
        for (int val : {3, 1, 4, 1, 5, 9, 2, 6}) {
            tracker.update(val);
        }
        std::cout << "最小值: " << tracker.get_min() << std::endl;
        std::cout << "最大值: " << tracker.get_max() << std::endl;
    }

}  // namespace ConceptUtilities

namespace ConceptsVsSFINAE {

    // 传统SFINAE方式
    template <typename T>
    std::enable_if_t<std::is_integral_v<T>, void> print_integral_sfinae(
        T value) {
        std::cout << "SFINAE - 整数: " << value << std::endl;
    }

    template <typename T>
    std::enable_if_t<std::is_floating_point_v<T>, void> print_floating_sfinae(
        T value) {
        std::cout << "SFINAE - 浮点: " << value << std::endl;
    }

    // 使用Concepts的现代方式
    template <std::integral T>
    void print_integral_concept(T value) {
        std::cout << "Concept - 整数: " << value << std::endl;
    }

    template <std::floating_point T>
    void print_floating_concept(T value) {
        std::cout << "Concept - 浮点: " << value << std::endl;
    }

    // 更复杂的约束比较
    namespace SFINAE_Version {
        template <typename T, typename = void>
        struct has_size : std::false_type {};

        template <typename T>
        struct has_size<T, std::void_t<decltype(std::declval<T>().size())>>
            : std::true_type {};

        template <typename T>
        std::enable_if_t<has_size<T>::value, void> print_size(
            const T& container) {
            std::cout << "SFINAE - 容器大小: " << container.size() << std::endl;
        }
    }  // namespace SFINAE_Version

    namespace Concepts_Version {
        template <typename T>
        concept HasSize = requires(T t) {
            { t.size() } -> std::convertible_to<size_t>;
        };

        template <HasSize T>
        void print_size(const T& container) {
            std::cout << "Concept - 容器大小: " << container.size()
                      << std::endl;
        }
    }  // namespace Concepts_Version

    void demonstrateComparison() {
        std::cout << "\n=== Concepts vs SFINAE 比较 ===" << std::endl;

        int i = 42;
        double d = 3.14;
        std::vector<int> vec{1, 2, 3};

        print_integral_sfinae(i);
        print_floating_sfinae(d);

        print_integral_concept(i);
        print_floating_concept(d);

        SFINAE_Version::print_size(vec);
        Concepts_Version::print_size(vec);

        std::cout << "\nConcepts的优势:" << std::endl;
        std::cout << "1. 更清晰的语法" << std::endl;
        std::cout << "2. 更好的错误信息" << std::endl;
        std::cout << "3. 可组合性" << std::endl;
        std::cout << "4. 自文档化" << std::endl;
    }

}  // namespace ConceptsVsSFINAE

namespace RealWorldExample {

    // 实际应用：通用算法库
    template <typename T>
    concept Numeric = std::integral<T> || std::floating_point<T>;

    template <typename T>
    concept Range = requires(T t) {
        t.begin();
        t.end();
    };

    template <typename R>
    concept NumericRange =
        Range<R> && Numeric<typename std::iterator_traits<
                        decltype(std::declval<R>().begin())>::value_type>;

    // 通用统计函数
    template <NumericRange R>
    auto calculate_statistics(const R& range) {
        using ValueType =
            typename std::iterator_traits<decltype(range.begin())>::value_type;

        struct Statistics {
            ValueType sum;
            ValueType mean;
            ValueType min;
            ValueType max;
            size_t count;
        };

        if (range.begin() == range.end()) {
            return Statistics{};
        }

        auto first = range.begin();
        auto last = range.end();

        Statistics stats;
        stats.sum = *first;
        stats.min = *first;
        stats.max = *first;
        stats.count = 1;

        for (auto it = std::next(first); it != last; ++it) {
            stats.sum += *it;
            if (*it < stats.min) stats.min = *it;
            if (*it > stats.max) stats.max = *it;
            ++stats.count;
        }

        stats.mean = stats.sum / static_cast<ValueType>(stats.count);
        return stats;
    }

    // 通用搜索函数
    template <Range R, typename Predicate>
        requires std::predicate<
            Predicate, typename std::iterator_traits<
                           decltype(std::declval<R>().begin())>::value_type>
    auto find_elements(const R& range, Predicate pred) {
        std::vector<
            typename std::iterator_traits<decltype(range.begin())>::value_type>
            result;

        for (const auto& item : range) {
            if (pred(item)) {
                result.push_back(item);
            }
        }
        return result;
    }

    void demonstrateRealWorld() {
        std::cout << "\n=== 实际应用示例 ===" << std::endl;

        std::vector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::vector<double> doubles{1.1, 2.2, 3.3, 4.4, 5.5};

        // 统计计算
        auto int_stats = calculate_statistics(numbers);
        auto double_stats = calculate_statistics(doubles);

        std::cout << "整数统计:" << std::endl;
        std::cout << "  总和: " << int_stats.sum << std::endl;
        std::cout << "  平均: " << int_stats.mean << std::endl;
        std::cout << "  最小: " << int_stats.min << std::endl;
        std::cout << "  最大: " << int_stats.max << std::endl;

        std::cout << "浮点统计:" << std::endl;
        std::cout << "  总和: " << double_stats.sum << std::endl;
        std::cout << "  平均: " << double_stats.mean << std::endl;

        // 条件搜索
        auto even_numbers =
            find_elements(numbers, [](int x) { return x % 2 == 0; });
        auto large_doubles =
            find_elements(doubles, [](double x) { return x > 3.0; });

        std::cout << "偶数: ";
        for (int n : even_numbers) std::cout << n << " ";
        std::cout << std::endl;

        std::cout << "大于3.0的浮点数: ";
        for (double d : large_doubles) std::cout << d << " ";
        std::cout << std::endl;
    }

}  // namespace RealWorldExample

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "     C++20 Concepts 教学示例" << std::endl;
    std::cout << "========================================" << std::endl;

    BasicConcepts::demonstrateBasicConcepts();
    ConceptConstrainedFunctions::demonstrateConstrainedFunctions();
    AdvancedConcepts::demonstrateAdvancedConcepts();
    ConceptUtilities::demonstrateUtilities();
    ConceptsVsSFINAE::demonstrateComparison();
    RealWorldExample::demonstrateRealWorld();

    std::cout << "\n重要提示：" << std::endl;
    std::cout << "1. Concepts提供编译时约束检查" << std::endl;
    std::cout << "2. 比SFINAE更清晰和易用" << std::endl;
    std::cout << "3. 支持概念组合和特化" << std::endl;
    std::cout << "4. 提供更好的错误信息" << std::endl;
    std::cout << "5. 使模板代码更加自文档化" << std::endl;

    return 0;
}