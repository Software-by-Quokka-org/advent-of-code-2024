#include <print>
#include <type_traits>
#include <string>
#include <vector>


constexpr std::vector<std::string> calls;

template<typename... Ts>
struct show_type;

template<typename... Elements>
struct list{};

template<auto V>
struct literal {
    constexpr static auto value = V;
};

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


template<auto... Elements>
struct input {
    using type = list<literal<char(Elements)>...>;
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
            return F<typename Func<T, R>::value>{};
        };
    };
};


template<template<class, class> typename Func, typename Init, typename... Elements>
struct fold<Func, Init, list<Elements...>> {
    template<typename T>
    using F = fold_helper<Func>::template F<T>;

    using value = decltype((F<Init>{} << ... << F<Elements>{}))::type;
};


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

template<typename T>
using is_digit = literal<0 <= T::value - '0' && T::value - '0' <= 9>;

template<typename T>
using as_digit = if_else<
    is_digit<T>, 
    literal<T::value - '0'>,
    none
    // typename if_else<
    //     literal<T::value >= 0 && T::value <= 9>,
    //     T,
    //     none>::type
>;


using data = input<
#include "input1.i"
>::type;


// using testdata = input<49, 50>::type;
using testdata = list<literal<'1'>, literal<'0'>, literal<'5'>, literal<' '>>;

template<typename L, typename R>
struct plus {
    using left = as_digit<L>::type;
    using right = as_digit<R>::type;
    using value = literal<left::value + right::value>;
};

template<typename Acc>
struct concatenator {
    using number = Acc;
};

using empty_concat = concatenator<none>;

template<typename In, typename Char>
struct concat {
    using digit = typename as_digit<Char>::type;

    struct IfNotNone {
        using type = concatenator<
            literal<In::number::value * 10 + digit::value>
        >;
    };

    using type = if_else<
        typename is_none<typename In::number>::type,
        concatenator<digit>, 
        IfNotNone
    >::type::type::number;


};

/*
Parse number from multiple digits, disregard spaces, parse number again, disregard newline
Create two sorted lists out of the numbers
  maybe: make it into one list of pairs
accumulate over abs(l1[i] - l2[i])

*/

int main() {
    // using answer = 
    // using answer = fold<plus, literal<'0'>, testdata>::value;
    // using answer = fold<read_number, empty_concat, testdata>::value;
    using answer = concat<concatenator<literal<5>>, literal<'3'>>::type;
    std::print("answer is {}\n", answer::value);
    return 0;
}