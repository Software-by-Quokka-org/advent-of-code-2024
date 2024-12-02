#include <limits>
#include <print>
#include "include/list.hpp"
#include "include/parsing.hpp"
#include "include/primitives.hpp"




using data = input<
#include "input1.i"
>::type;


// using testdata = input<49, 50>::type;


template<typename List>
struct both {
    using first = parse_int<List>;
    using second = parse_int<typename lstrip<typename first::rest>::type>;

    using first_value = first::type;
    using second_value = second::type;
};

template<typename Lowest, typename Rest>
struct keep_lowest {
    using lowest = Lowest;
    using rest = Rest;
};

using init_keeper = keep_lowest<literal<std::numeric_limits<int>::max()>, list<>>;

template<typename List, typename Keeper>
struct extract_lowest {};

template<typename Keeper>
struct extract_lowest<list<>, Keeper> {
    using rest = list<>;
    using type = Keeper;
};

// template<typename Element, typename... Elements, template<typename, typename...> typename Keeper, typename Lowest, typename... StartSeq>
// struct extract_lowest<list<Element, Elements...>, keep_lowest<Lowest, list<StartSeq..., Elements...>>> {
//     // Element is literal<123>
//     // using Keeper = keep_lowest<Lowest, list<StartSeq..., Elements...>>;

//     struct LessThan {
//         using advance = extract_lowest<list<Elements...>, keep_lowest<Element, list<Elements...>>>;
//         using rest = typename advance::rest;
//         using type = typename advance::type;
//     };

//     struct GreaterEqualThan {
//         using advance = extract_lowest<list<Elements...>, Keeper>;
//         using rest = typename advance::rest;
//         using type = typename advance::type;
//     };

//     using cond = typename if_else<
//         literal<Element::value < Keeper::lowest::value>,
//         LessThan,
//         GreaterEqualThan
//     >::type;

//     using rest = typename cond::rest;
//     using type = typename cond::type;
// };

// template<typename T>
// struct solve {
//     using type = typename fold<concat, empty_concat, T>::type::number;
// };

/*
Parse number from multiple digits, disregard spaces, parse number again, disregard newline
Create two sorted lists out of the numbers
  maybe: make it into one list of pairs
accumulate over abs(l1[i] - l2[i])

*/

int main() {
    // Testing "extract_lowest"
    using testdata = value_list<3,5,2,1,4>::type;
    using lowest1 = extract_lowest<testdata, init_keeper>;
    // show_type<lowest1::type> show;
    // using lowest2 = extract_lowest<lowest1::type::rest>;
    // std::print("lowest is {}, second lowest is {}\n", lowest1::type::lowest::value, lowest2::type::lowest::value);
    // Testing "both"
    // using testdata = list<literal<'1'>, literal<'5'>, literal<' '>, literal<' '>, literal<'1'>, literal<'4'>, literal<'\n'>>;
    // using answer = both<testdata>;
    // std::print("answer is {} and {}\n", answer::first_value::value, answer::second_value::value);
    return 0;
}