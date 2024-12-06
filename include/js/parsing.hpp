#pragma once

#include "list.hpp"
#include "primitives.hpp"
#include <type_traits>



template<auto... Elements>
struct input {
    using type = list<literal<char(Elements)>...>;
};

template<typename T>
using is_digit = literal<0 <= T::value - '0' && T::value - '0' <= 9>;

template<typename T>
using as_digit = if_else<
    is_digit<T>, 
    literal<T::value - '0'>,
    none
>;

template<typename String, typename Accum=literal<0>>
struct parse_int {};

template<typename Accum>
struct parse_int<list<>, Accum> {
    using rest = list<>;
    using type = Accum;
};

template<typename Element, typename... Elements, typename Accum>
struct parse_int<list<Element, Elements...>, Accum> {
    using digit = typename as_digit<Element>::type;

    struct IfDigit {
        using advance = parse_int<list<Elements...>, literal<10*Accum::value + digit::value>>;
        using rest = typename advance::rest;
        using type = typename advance::type;
    };

    struct IfNotDigit {
        using rest = list<Element, Elements...>;
        using type = Accum;
    };

    using cond = typename if_else<
        typename is_none<digit>::type,
        IfNotDigit,
        IfDigit
    >::type;

    using rest = typename cond::rest;
    using type = typename cond::type;
};

template<typename String, typename Char = literal<' '>>
struct lstrip {};

template<typename Char>
struct lstrip<list<>, Char> {
    using rest = list<>;
    // using type = Char;
};

template<typename Element, typename... Elements, typename Char>
struct lstrip<list<Element, Elements...>, Char> {

    struct IfChar {
        using type = lstrip<list<Elements...>, Char>::type;
    };

    struct IfNotChar {
        using type = list<Element, Elements...>;
    };

    using type = typename if_else<
        literal<std::is_same_v<Element, Char>>,
        IfChar,
        IfNotChar
    >::type::type;
};

struct TestLstrip {
    using expected = list<literal<'c'>>;
    using actual = lstrip<decltype("aac"_str), literal<'a'>>::type;
    static_assert(std::is_same_v<expected, actual>);
};


template<typename Accum, typename Inner, typename Outer>
struct IntMatrix {
    using accum = Accum;
    using inner = Inner;
    using outer = Outer;
};

using empty_matrix = IntMatrix<literal<0>, list<>, list<>>;

template<typename In, typename Char>
struct parse_line_numbers_func {
    using digit = as_digit<Char>::type;

    struct IfNewLine {
        using type = IntMatrix<
            literal<0>,
            list<>,
            typename append<typename In::outer, 
                            typename append<typename In::inner, 
                                            typename In::accum>::type>::type
        >;
    };

    struct IfSpace {
        using type = IntMatrix<
            literal<0>,
            typename append<typename In::inner, typename In::accum>::type,
            typename In::outer
        >;
    };

    struct Else {
        using type = IntMatrix<
            literal<In::accum::value * 10 + digit::value>,
            typename In::inner,
            typename In::outer
        >;
    };

    using type = if_else<
        literal<Char::value == '\n'>,
        IfNewLine,
        typename if_else<
            literal<Char::value == ' '>,
            IfSpace,
            Else
        >::type
    >::type::type;
};

template<typename T>
using parse_lines = fold<parse_line_numbers_func, empty_matrix, T>::type::outer;

struct TestParseLines {
    using data = input<
        0x32, 0x37, 0x20, 0x31, 0x32, 0x36, 0x20, 0x34, 0x34, 0x20, 0x32, 0x20,
        0x31, 0x0a, 0x31, 0x20, 0x32, 0x33, 0x20, 0x37, 0x35, 0x34, 0x20, 0x36,
        0x38, 0x20, 0x34, 0x39, 0x0a, 0x39, 0x35, 0x36, 0x20, 0x37, 0x33, 0x20,
        0x36, 0x20, 0x32, 0x32, 0x20, 0x31, 0x32, 0x0a
    >::type;
    using actual = parse_lines<data>;

    using row1 = value_list<27, 126, 44, 2, 1>::type;
    using row2 = value_list<1, 23, 754, 68, 49>::type;
    using row3 = value_list<956, 73, 6, 22, 12>::type;
    using expected = list<row1, row2, row3>;

    static_assert(std::is_same_v<actual, expected>);
};