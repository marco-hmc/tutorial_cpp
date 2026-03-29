#ifndef TUPLE_H
#define TUPLE_H

#include <cstddef>
#include <utility>

// 基类模板，用于递归继承
template <std::size_t Index, typename T>
struct TupleElement {
    T value;

    TupleElement() = default;

    TupleElement(const T& val) : value(val) {}

    TupleElement(T&& val) : value(std::move(val)) {}
};

// 递归继承的 Tuple 类模板
template <typename... Types>
struct Tuple;

template <std::size_t... Indices, typename... Types>
struct Tuple<std::index_sequence<Indices...>, Types...>
    : TupleElement<Indices, Types>... {
    Tuple() = default;

    Tuple(const Types&... values) : TupleElement<Indices, Types>(values)... {}

    Tuple(Types&&... values)
        : TupleElement<Indices, Types>(std::move(values))... {}
};

// 辅助别名模板
template <typename... Types>
using TupleBase = Tuple<std::index_sequence_for<Types...>, Types...>;

template <typename... Types>
struct Tuple : TupleBase<Types...> {
    using TupleBase<Types...>::TupleBase;
};

// 获取 Tuple 中元素的类型
template <std::size_t Index, typename T>
struct TupleElementType;

template <std::size_t Index, typename Head, typename... Tail>
struct TupleElementType<Index, Tuple<Head, Tail...>>
    : TupleElementType<Index - 1, Tuple<Tail...>> {};

template <typename Head, typename... Tail>
struct TupleElementType<0, Tuple<Head, Tail...>> {
    using Type = Head;
};

// 获取 Tuple 中元素的引用
template <std::size_t Index, typename... Types>
decltype(auto) get(Tuple<Types...>& tuple) {
    return static_cast<TupleElement<
        Index, typename TupleElementType<Index, Tuple<Types...>>::Type>&>(tuple)
        .value;
}

template <std::size_t Index, typename... Types>
decltype(auto) get(const Tuple<Types...>& tuple) {
    return static_cast<const TupleElement<
        Index, typename TupleElementType<Index, Tuple<Types...>>::Type>&>(tuple)
        .value;
}

template <std::size_t Index, typename... Types>
decltype(auto) get(Tuple<Types...>&& tuple) {
    return std::move(
        static_cast<TupleElement<
            Index, typename TupleElementType<Index, Tuple<Types...>>::Type>&>(
            tuple)
            .value);
}

#endif  // TUPLE_H