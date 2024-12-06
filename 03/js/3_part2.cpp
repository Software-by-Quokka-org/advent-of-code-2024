
#include <limits>
#include <print>
#include <type_traits>
#include "js/list.hpp"
#include "js/parsing.hpp"
#include "js/primitives.hpp"

using data = input<
#include "input3.i"
>::type;


using testdata = input<
  0x3b, 0x28, 0x2c, 0x29, 0x3c, 0x6d, 0x75, 0x6c, 0x28, 0x35, 0x39, 0x35,
  0x2c, 0x31, 0x31, 0x30, 0x29, 0x7e, 0x20, 0x23, 0x28, 0x73, 0x65, 0x6c,
  0x65, 0x63, 0x74, 0x28, 0x29, 0x2d, 0x3f, 0x77, 0x68, 0x6f, 0x28, 0x29,
  0x3a, 0x6d, 0x75, 0x6c, 0x28, 0x37, 0x33, 0x32, 0x2c, 0x37, 0x32, 0x39,
  0x29, 0x2b, 0x2f, 0x3b, 0x25, 0x40, 0x6d, 0x75, 0x6c, 0x28, 0x39, 0x32,
  0x34, 0x2c, 0x37, 0x30, 0x30, 0x29
>::type;

struct S0 {};
struct Sm {};
struct Smu {};
struct Smul {};
struct Smul_p {};
struct Sd {};
struct Sdo {};
struct Sdon {};
struct Sdona {};
struct Sdonat {};

template<typename LHS> struct Smul_p_a { using lhs = LHS; };
template<typename LHS> struct Smul_p_a_c {using lhs = LHS; };
template<typename LHS, typename RHS> struct Smul_p_a_c_b { using lhs = LHS; using rhs = RHS; };


template<typename State, typename Element>
struct next_state {
    using type = S0;
};

// Spell "mul("
template<typename State> struct next_state<State, literal<'m'>> { using type = Sm; };
template<> struct next_state<Sm, literal<'u'>> { using type = Smu; };
template<> struct next_state<Smu, literal<'l'>> { using type = Smul; };
template<> struct next_state<Smul, literal<'('>> { using type = Smul_p; };

// Spell do or don't
template<typename State> struct next_state<State, literal<'d'>> { using type = Sd; };
template<> struct next_state<Sd, literal<'o'>> { using type = Sdo; };
template<> struct next_state<Sdo, literal<'n'>> { using type = Sdon; };
template<> struct next_state<Sdon, literal<'\''>> { using type = Sdona; };
template<> struct next_state<Sdona, literal<'t'>> { using type = Sdonat; };


// Left-hand number start
template<> struct next_state<Smul_p, literal<'1'>> { using type = Smul_p_a<literal<1>>; };
template<> struct next_state<Smul_p, literal<'2'>> { using type = Smul_p_a<literal<2>>; };
template<> struct next_state<Smul_p, literal<'3'>> { using type = Smul_p_a<literal<3>>; };
template<> struct next_state<Smul_p, literal<'4'>> { using type = Smul_p_a<literal<4>>; };
template<> struct next_state<Smul_p, literal<'5'>> { using type = Smul_p_a<literal<5>>; };
template<> struct next_state<Smul_p, literal<'6'>> { using type = Smul_p_a<literal<6>>; };
template<> struct next_state<Smul_p, literal<'7'>> { using type = Smul_p_a<literal<7>>; };
template<> struct next_state<Smul_p, literal<'8'>> { using type = Smul_p_a<literal<8>>; };
template<> struct next_state<Smul_p, literal<'9'>> { using type = Smul_p_a<literal<9>>; };

// Left-hand number continue
template<typename LHS> struct next_state<Smul_p_a<LHS>, literal<'0'>> { using type = Smul_p_a<literal<LHS::value * 10 + 0>>; };
template<typename LHS> struct next_state<Smul_p_a<LHS>, literal<'1'>> { using type = Smul_p_a<literal<LHS::value * 10 + 1>>; };
template<typename LHS> struct next_state<Smul_p_a<LHS>, literal<'2'>> { using type = Smul_p_a<literal<LHS::value * 10 + 2>>; };
template<typename LHS> struct next_state<Smul_p_a<LHS>, literal<'3'>> { using type = Smul_p_a<literal<LHS::value * 10 + 3>>; };
template<typename LHS> struct next_state<Smul_p_a<LHS>, literal<'4'>> { using type = Smul_p_a<literal<LHS::value * 10 + 4>>; };
template<typename LHS> struct next_state<Smul_p_a<LHS>, literal<'5'>> { using type = Smul_p_a<literal<LHS::value * 10 + 5>>; };
template<typename LHS> struct next_state<Smul_p_a<LHS>, literal<'6'>> { using type = Smul_p_a<literal<LHS::value * 10 + 6>>; };
template<typename LHS> struct next_state<Smul_p_a<LHS>, literal<'7'>> { using type = Smul_p_a<literal<LHS::value * 10 + 7>>; };
template<typename LHS> struct next_state<Smul_p_a<LHS>, literal<'8'>> { using type = Smul_p_a<literal<LHS::value * 10 + 8>>; };
template<typename LHS> struct next_state<Smul_p_a<LHS>, literal<'9'>> { using type = Smul_p_a<literal<LHS::value * 10 + 9>>; };
// Left-hand number end
template<typename LHS> struct next_state<Smul_p_a<LHS>, literal<','>> { using type = Smul_p_a_c<LHS>; };

// Right-hand number start
template<typename LHS> struct next_state<Smul_p_a_c<LHS>, literal<'1'>> { using type = Smul_p_a_c_b<LHS, literal<1>>; };
template<typename LHS> struct next_state<Smul_p_a_c<LHS>, literal<'2'>> { using type = Smul_p_a_c_b<LHS, literal<2>>; };
template<typename LHS> struct next_state<Smul_p_a_c<LHS>, literal<'3'>> { using type = Smul_p_a_c_b<LHS, literal<3>>; };
template<typename LHS> struct next_state<Smul_p_a_c<LHS>, literal<'4'>> { using type = Smul_p_a_c_b<LHS, literal<4>>; };
template<typename LHS> struct next_state<Smul_p_a_c<LHS>, literal<'5'>> { using type = Smul_p_a_c_b<LHS, literal<5>>; };
template<typename LHS> struct next_state<Smul_p_a_c<LHS>, literal<'6'>> { using type = Smul_p_a_c_b<LHS, literal<6>>; };
template<typename LHS> struct next_state<Smul_p_a_c<LHS>, literal<'7'>> { using type = Smul_p_a_c_b<LHS, literal<7>>; };
template<typename LHS> struct next_state<Smul_p_a_c<LHS>, literal<'8'>> { using type = Smul_p_a_c_b<LHS, literal<8>>; };
template<typename LHS> struct next_state<Smul_p_a_c<LHS>, literal<'9'>> { using type = Smul_p_a_c_b<LHS, literal<9>>; };

// Right-hand number continue
template<typename LHS, typename RHS> struct next_state<Smul_p_a_c_b<LHS, RHS>, literal<'0'>> { using type = Smul_p_a_c_b<LHS, literal<RHS::value * 10 + 0>>; };
template<typename LHS, typename RHS> struct next_state<Smul_p_a_c_b<LHS, RHS>, literal<'1'>> { using type = Smul_p_a_c_b<LHS, literal<RHS::value * 10 + 1>>; };
template<typename LHS, typename RHS> struct next_state<Smul_p_a_c_b<LHS, RHS>, literal<'2'>> { using type = Smul_p_a_c_b<LHS, literal<RHS::value * 10 + 2>>; };
template<typename LHS, typename RHS> struct next_state<Smul_p_a_c_b<LHS, RHS>, literal<'3'>> { using type = Smul_p_a_c_b<LHS, literal<RHS::value * 10 + 3>>; };
template<typename LHS, typename RHS> struct next_state<Smul_p_a_c_b<LHS, RHS>, literal<'4'>> { using type = Smul_p_a_c_b<LHS, literal<RHS::value * 10 + 4>>; };
template<typename LHS, typename RHS> struct next_state<Smul_p_a_c_b<LHS, RHS>, literal<'5'>> { using type = Smul_p_a_c_b<LHS, literal<RHS::value * 10 + 5>>; };
template<typename LHS, typename RHS> struct next_state<Smul_p_a_c_b<LHS, RHS>, literal<'6'>> { using type = Smul_p_a_c_b<LHS, literal<RHS::value * 10 + 6>>; };
template<typename LHS, typename RHS> struct next_state<Smul_p_a_c_b<LHS, RHS>, literal<'7'>> { using type = Smul_p_a_c_b<LHS, literal<RHS::value * 10 + 7>>; };
template<typename LHS, typename RHS> struct next_state<Smul_p_a_c_b<LHS, RHS>, literal<'8'>> { using type = Smul_p_a_c_b<LHS, literal<RHS::value * 10 + 8>>; };
template<typename LHS, typename RHS> struct next_state<Smul_p_a_c_b<LHS, RHS>, literal<'9'>> { using type = Smul_p_a_c_b<LHS, literal<RHS::value * 10 + 9>>; };


template<typename State, typename Element>
struct match_whole {
    using type = literal<0>;
};
// Right-hand number end
template<typename LHS, typename RHS> struct match_whole<Smul_p_a_c_b<LHS, RHS>, literal<')'>> { using type = literal<LHS::value * RHS::value>; };

template<typename MatchState, typename Enabled, typename Accum>
struct MultiplierState {
    using state = MatchState;
    using enabled = Enabled;
    using accum = Accum;
};

using empty_state = MultiplierState<S0, literal<true>, literal<0>>;

template<typename In, typename Char>
struct Func {
    using new_state = next_state<typename In::state, Char>::type;
    using value = match_whole<typename In::state, Char>::type;

    using type = MultiplierState<
        new_state,
        typename if_else<
            literal<std::is_same_v<new_state, Sdo>>,
            literal<true>,
            typename if_else<
                literal<std::is_same_v<new_state, Sdonat>>,
                literal<false>,
                typename In::enabled
            >::type
        >::type,
        literal<In::accum::value + value::value * int(In::enabled::value)>
        // typename or_else<typename In::state, literal<In::accum::value + value::value>>::type
    >; 
};


struct Test {
    static_assert(std::is_same_v<
        typename fold<Func, empty_state, typename input<
            'm', 'u', 'l', '(', '3', ',','2', ')'
        >::type>::type,
        MultiplierState<S0, literal<true>, literal<6>>
    >);
    static_assert(std::is_same_v<
        typename fold<Func, empty_state, typename input<
            'd', 'o', 'n', '\'', 't', 'm', 'u', 'l', '(', '3', ',','2', ')'
        >::type>::type,
        MultiplierState<S0, literal<false>, literal<0>>
    >);

    using res = fold<Func, empty_state, testdata>::type::accum;
    static_assert(res::value == 1245878);
};

template<typename T>
struct solve {
    using type = fold<Func, empty_state, T>::type::accum;
};

int main() {
    using answer = solve<data>::type;
    std::print("answer is {}\n", answer::value);
    return 0;
}