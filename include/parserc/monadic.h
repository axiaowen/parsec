#include "trait.h"

namespace parsec {

// pipe operator
template <Parser P, typename F>
constexpr auto operator|(P&& parser, F&& f) {
  return [parser, f](const Input& input) {
    return f(parser(input));
  };
}

// converts the parser's result to anther type
template <typename F>
constexpr auto to(F&& f) {
  return [f](auto&& result) {
    return result.and_then([f](auto&& result) {
      auto&& [value, rest] = result;

      using V = decltype(value);
      using R = std::invoke_result_t<F, V>;
      static_assert(not std::is_same_v<R, void>,
          "map requires the return type of F not to be void.");

      return Result<R>{ f(value) };
    });
  };
}

// map a function over the result of a parser
template <typename F>
constexpr auto map(F&& f) {
  return [f](auto&& result) {
    return result.map([f](auto&& result) {
      auto&& [value, rest] = result;

      using V = decltype(value);
      using R = std::invoke_result_t<F, V>;
      static_assert(not std::is_same_v<R, void>,
          "map requires the return type of F not to be void.");

      return Output<R>{ f(value), rest };
    });
  };
}

// applies a function over the input if the parser failed
template <std::invocable<> F>
constexpr auto or_else(F&& f) {
  return [f](auto&& result) {
    return result.or_else(f);
  };
}

// applies a function over the result of a parser
template <typename F>
constexpr auto and_then(F&& f) {
  return [f](auto&& result) {
    return result.and_then(f);
  };
}

}  // namespace parsec