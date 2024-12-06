
#include <limits>
#include <print>
#include "js/list.hpp"
#include "js/parsing.hpp"
#include "js/primitives.hpp"

using data = input<
#include "input2.i"
>::type;

using testdata = input<
  0x37, 0x20, 0x36, 0x20, 0x34, 0x20, 0x32, 0x20, 0x31, 0x0a, 0x31, 0x20,
  0x32, 0x20, 0x37, 0x20, 0x38, 0x20, 0x39, 0x0a, 0x39, 0x20, 0x37, 0x20,
  0x36, 0x20, 0x32, 0x20, 0x31, 0x0a, 0x31, 0x20, 0x33, 0x20, 0x32, 0x20,
  0x34, 0x20, 0x35, 0x0a, 0x38, 0x20, 0x36, 0x20, 0x34, 0x20, 0x34, 0x20,
  0x31, 0x0a, 0x31, 0x20, 0x33, 0x20, 0x36, 0x20, 0x37, 0x20, 0x39, 0x0a
>::type;

enum Order {
    Neither,
    Asc,
    Desc
};

enum Prev {
    NoValue = std::numeric_limits<int>::max()
};


template<typename PrevDigit, typename Ordering, typename Continuous>
struct LineState {
    using prev = PrevDigit;
    using order = Ordering;
    using cont = Continuous; 
};

using empty_line_state = LineState<literal<Prev::NoValue>, literal<Order::Neither>, literal<true>>;

template<typename In, typename Number>
struct number_check {
    template<typename T>
    using dst = typename if_else<
        literal<T::value == Order::Asc>,
        literal<Number::value - In::prev::value>,
        literal<In::prev::value - Number::value>
    >::type;
    struct IfNoPrev {
        using type = LineState<Number, typename In::order, typename In::cont>;
    };

    struct IfNoOrder {
        using order = if_else<
            literal<In::prev::value < Number::value>,
            literal<Order::Asc>,
            literal<Order::Desc>
        >::type;
        using type = LineState<
            Number,
            order,
            typename if_else<
                literal<order::value == Order::Asc && 1 <= dst<order>::value && dst<order>::value <= 3 ||
                        order::value == Order::Desc && 1 <= dst<order>::value && dst<order>::value <= 3>,
                literal<true>,
                literal<false>
            >::type
        >;
    };

    struct ElseCheckOrder {
        using type = LineState<
            Number,
            typename In::order,
            typename if_else<
                literal<1 <= dst<typename In::order>::value && dst<typename In::order>::value <= 3>,
                typename In::cont,
                literal<false>
            >::type
        >;
    };

    using type = if_else<
        literal<In::prev::value == Prev::NoValue>,
        IfNoPrev,
        typename if_else<
            literal<In::order::value == Order::Neither>,
            IfNoOrder,
            ElseCheckOrder
        >::type
    >::type::type;
};


struct TestNumberCheck {
    using Line = value_list<5, 3, 2>::type;
    using actual = typename fold<number_check, empty_line_state, Line>::type;
    static_assert(actual::cont::value == true);
};

template<typename In, typename Line>
struct line_check {
    using line_res = typename fold<number_check, empty_line_state, Line>::type;
    using type = literal<In::value + int(line_res::cont::value)>;
};


template<typename T>
struct solve {
    using type = typename fold<line_check, literal<0>, T>::type;
};

struct TestSolution {
    using answer = solve<parse_lines<testdata>>::type;
    static_assert(answer::value == 2);
};

int main() {
    using answer = solve<parse_lines<data>>::type; // Real data, takes ~2 minutes to compile
    std::print("Answer is {}\n", answer::value);
    return 0;
}