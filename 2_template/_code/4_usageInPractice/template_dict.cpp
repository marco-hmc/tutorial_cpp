/*
    字典结构（Dictionary Structure）
    字典结构（或映射结构）在元编程中常常用来表示键值对。
    可以通过模板元编程实现类似于哈希表的结构，或者通过递归模板和类型萃取构造静态的字典结构。
*/

#include <iostream>
#include <type_traits>

namespace dict_meta {

    template <auto Key, typename Value>
    struct pair {
        static constexpr auto key = Key;
        using value_type = Value;
    };

    template <typename... Pairs>
    struct dictionary;

    template <auto Key, typename Dict>
    struct find_value;

    template <auto Key, typename Head, typename... Tail>
    struct find_value<Key, dictionary<Head, Tail...>> {
        using type = std::conditional_t<
            (Key == Head::key), typename Head::value_type,
            typename find_value<Key, dictionary<Tail...>>::type>;
    };

    template <auto Key>
    struct find_value<Key, dictionary<>> {
        using type = void;
    };
}  // namespace dict_meta

int main() {
    using namespace dict_meta;

    using my_dict =
        dictionary<pair<1, const char*>, pair<2, int>, pair<3, double>>;

    using t1 = find_value<1, my_dict>::type;
    using t2 = find_value<2, my_dict>::type;
    using t3 = find_value<3, my_dict>::type;
    using t4 = find_value<99, my_dict>::type;

    std::cout << std::boolalpha;
    std::cout << "t1 is const char*: " << std::is_same<t1, const char*>::value
              << '\n';
    std::cout << "t2 is int: " << std::is_same<t2, int>::value << '\n';
    std::cout << "t3 is double: " << std::is_same<t3, double>::value << '\n';
    std::cout << "t4 is void: " << std::is_same<t4, void>::value << '\n';

    return 0;
}
