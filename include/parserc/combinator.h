#pragma once
#include <array>
#include <vector>

#include "monadic.h"

namespace parsec {

// consumes no input and always succeeds with given value.
template <typename V, typename... Args>
  requires std::is_constructible_v<V, Args...>
constexpr auto success(Args&&... args) {
  return [=](const Input& input) -> ParserResult<V> {
    return { { V(std::forward<Args>(args)...), input } };
  };
}

// return the result of the parser if it satisfies a function.
template <Parser P, typename F>
  requires std::predicate<F, invoke_parser_result_t<P>>
constexpr auto predict(P&& parser, F&& f) {
  using R = ParserResult<invoke_parser_result_t<P>>;

  return parser | and_then([f](auto&& result) {
    auto&& [value, rest] = result;
    if (f(value)) {
      return R{ result };
    }
    return R{ std::unexpect, "parser does not satisfy." };
  });
}

// matches a sequence of parsers in the defined order.
template <Parser P1, Parser P2>
constexpr auto seq(P1&& parser1, P2&& parser2) {
  using V1 = invoke_parser_result_t<P1>;
  using V2 = invoke_parser_result_t<P2>;
  using R = Output<std::tuple<V1, V2>>;

  return parser1 | and_then([parser2](auto&& result) {
    auto&& [value1, rest] = result;
    return parser2(rest).map([value1](auto&& result) {
      auto&& [value2, rest] = result;
      return R{ { value1, value2 }, rest };
    });
  });
}

// operator for seq.
template <Parser P1, Parser P2>
constexpr auto operator+(P1&& parser1, P2&& parser2) {
  return seq(parser1, parser2);
}

// tries to apply the parsers in order until one of them succeeds.
template <Parser P1, Parser P2>
  requires std::same_as<invoke_parser_result_t<P1>, invoke_parser_result_t<P2>>
constexpr auto choice(P1&& parser1, P2&& parser2) {
  return [parser1, parser2](const Input& input) {
    return parser1(input).or_else([parser2, input]() {
      return parser2(input);
    });
  };
}

// operator for choice.
template <Parser P1, Parser P2>
constexpr auto operator||(P1&& parser1, P2&& parser2) {
  return choice(parser1, parser2);
}

// matches two parsers and accepts the result from the left side.
template <Parser P1, Parser P2>
constexpr auto left(P1&& parser1, P2&& parser2) {
  return (parser1 + parser2) | map([](auto&& result) {
    return std::get<0>(result);
  });
}

// matches two parsers and accepts the result from the right side.
template <Parser P1, Parser P2>
constexpr auto right(P1&& parser1, P2&& parser2) {
  return (parser1 + parser2) | map([](auto&& result) {
    return std::get<1>(result);
  });
}

// matches three parsers and accepts the result from the middle one.
template <Parser P1, Parser P2, Parser P3>
constexpr auto between(P1&& parser1, P2&& parser2, P3&& parser3) {
  return (parser1 + parser2 + parser3) | map([](auto&& result) {
    return std::get<1>(std::get<0>(result));
  });
}

// matches a parser multiple times, can be a matched 0 times.
template <Parser P>
constexpr auto many(P&& parser) {
  using V = invoke_parser_result_t<P>;
  using R = ParserResult<std::vector<V>>;

  return [parser](const Input& input) {
    Input rest = input;
    std::vector<V> list;
    while (auto result = parser(rest)) {
      if (!result.has_value()) {
        break;
      }

      rest = std::get<1>(result.value());
      list.emplace_back(std::get<0>(result.value()));
    }
    return R{ { list, rest } };
  };
}

// matches a parser in a fixed amount of times.
template <std::size_t N, Parser P>
constexpr auto many(P&& parser) {
  using V = invoke_parser_result_t<P>;
  using R = ParserResult<std::array<V, N>>;

  return [parser](const Input& input) {
    Input rest = input;
    std::array<V, N> ret;
    for (std::size_t i = 0; i < N; i++) {
      auto result = parser(rest);
      if (!result.has_value()) {
        return R{ std::unexpect, "many<> dismatches." };
      }

      rest = std::get<1>(result.value());
      ret[i] = std::get<0>(result.value());
    }
    return R{ { ret, rest } };
  };
}

// matches a parser multiple times at least one time.
template <Parser P>
constexpr auto many1(P&& parser) {
  using V = invoke_parser_result_t<P>;

  return (parser + many(parser)) | map([](auto&& result) {
    auto&& [first, others] = result;

    std::vector<V> ret;
    ret.reserve(others.size() + 1);
    ret.insert(ret.end(), first);
    ret.insert(ret.end(), others.begin(), others.end());
    return ret;
  });
}

// matches a parser separated by another parser at lease one time.
template <Parser P, Parser Sep>
constexpr auto sepby1(P&& parser, Sep&& sep) {
  using V = invoke_parser_result_t<P>;

  return (parser + many(sep + parser)) | map([](auto&& result) {
    auto&& [first, others] = result;

    std::vector<V> ret;
    ret.reserve(others.size() + 1);
    ret.emplace_back(first);
    for (auto& [_, other] : others) {
      ret.emplace_back(other);
    }
    return ret;
  });
}

// matches a parser separated by another parser, can be a matched 0 times.
template <Parser P, Parser Sep>
constexpr auto sepby(P&& parser, Sep&& sep) {
  using R = std::vector<invoke_parser_result_t<P>>;

  return sepby1(parser, sep) || success<R>();
}

// matches a parser separated by another parser in a fixed amount of times.
template <std::size_t N, Parser P, Parser Sep>
constexpr auto sepby(P&& parser, Sep&& sep) {
  using V = invoke_parser_result_t<P>;
  using R = ParserResult<std::array<V, N>>;

  return [parser, sep](const Input& input) {
    Input rest = input;
    std::array<V, N> ret;
    for (std::size_t i = 0; i < N; i++) {
      if (i != 0) {
        auto result = sep(rest);
        if (!result.has_value()) {
          return R{ std::unexpect, "sepby<> mismatches sep." };
        }
        rest = std::get<1>(result.value());
      }

      auto result = parser(rest);
      if (!result.has_value()) {
        return R{ std::unexpect, "sepby<> mismatches parser." };
      }

      rest = std::get<1>(result.value());
      ret[i] = std::get<0>(result.value());
    }

    return R{ { ret, rest } };
  };
}

// matches the end of the input.
template <Parser P>
constexpr auto eof(P&& parser) {
  using R = ParserResult<invoke_parser_result_t<P>>;

  return [parser](const Input& input) {
    auto result = parser(input);
    if (result.has_value()) {
      if (!std::get<1>(result.value()).empty()) {
        return R{ std::unexpect, "not end of input" };
      }
    }
    return result;
  };
}

}  // namespace parsec