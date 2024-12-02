#pragma once

#include "primitives.hpp"

#include <type_traits>

template<typename... Elements>
struct list{};

template<auto... Elements>
struct value_list {
    using type = list<literal<Elements>...>;
};

template<typename List, typename Element>
struct append {};

template<typename... Elements, typename Element>
struct append<list<Elements...>, Element> {
    using type = list<Elements..., Element>;
};


struct TestAppend {
    using A = value_list<1,2,3>::type;
    using E = value_list<1,2,3,4>::type;
    static_assert(std::is_same_v<append<A, literal<4>>::type, E>);
};

template<typename List, typename Element>
struct prepend {};

template<typename... Elements, typename Element>
struct prepend<list<Elements...>, Element> {
    using type = list<Element, Elements...>;
};

struct TestPrepend {
    using A = value_list<1,2,3>::type;
    using E = value_list<4,1,2,3>::type;
    static_assert(std::is_same_v<prepend<A, literal<4>>::type, E>);
};

template<typename List>
struct head {};

template<typename Head, typename... Elements>
struct head<list<Head, Elements...>> {
    using type = Head;
};

template<typename List>
struct tail {};

template<typename Tail, typename... Elements>
struct tail<list<Tail, Elements...>> {
    using type = list<Elements...>;
};


template<typename T, T...chars>
constexpr list<literal<chars>...> operator""_str() {return {};}

struct TestOpString {
    using expected = list<literal<'a'>, literal<'b'>, literal<'c'>>;
    using actual = decltype("abc"_str);
    static_assert(std::is_same_v<actual, expected>);
};




template<template<class, class> typename Func, typename Init, typename List>
struct fold {};

template<template<class, class> typename Func>
struct fold_helper {
    template<typename T>
    struct F {
        using type = T;

        template<typename R>
        auto operator<<(F<R>) {
            return F<typename Func<T, R>::type>{};
        };
    };
};


template<template<class, class> typename Func, typename Init, typename... Elements>
struct fold<Func, Init, list<Elements...>> {
    template<typename T>
    using F = fold_helper<Func>::template F<T>;

    using type = decltype((F<Init>{} << ... << F<Elements>{}))::type;
};


