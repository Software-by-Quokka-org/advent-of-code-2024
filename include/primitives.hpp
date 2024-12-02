#pragma once


#include <concepts>
template<typename... Ts>
struct show_type;


template<auto V>
struct literal {
    constexpr static auto value = V;
};

// template<auto L, auto R>
// concept Comparable = requires(L l, R r) {
    
// };

template<typename L, typename R> 
requires std::same_as<typename L::value, int> && std::same_as<typename R::value, int>
constexpr bool operator<(L l, R r) {
    return L::value < R::value;
}


template<typename T>
struct is_not {};

template<>
struct is_not<literal<false>> {
    using type = literal<true>;
};

template<>
struct is_not<literal<true>> {
    using type = literal<false>;
};

struct none;

template<typename T>
struct is_none { using type = literal<false>; };

template<>
struct is_none<none> { using type = literal<true>; };

template<typename T>
using is_not_none = is_not<typename is_none<T>::type>;

template<typename Cond, typename Then, typename Else>
struct if_else {};

template<typename Then, typename Else>
struct if_else<literal<true>, Then, Else> {
    using type = Then;
};

template<typename Then, typename Else>
struct if_else<literal<false>, Then, Else> {
    using type = Else;
};

template<typename L, typename R>
struct or_else { using type = L; };

template<typename R>
struct or_else<none, R> { using type = R; };