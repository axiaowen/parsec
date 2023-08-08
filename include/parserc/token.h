#pragma once
#include "character.h"
#include "combinator.h"

namespace parsec {

constexpr auto symbol(const Input& token) {
  using R = ParserResult<std::string_view>;
  return [token](const std::string_view& input) {
    if (input.starts_with(token)) {
      auto rest = input.substr(token.size());
      return R{ { token, rest } };
    }
    return R{ std::unexpect, "symbol dismatches." };
  };
}

template <typename T>
constexpr auto decimal = many1(digit) | map([](auto&& result) {
  T value = 0;
  for (auto c : result) {
    value *= 10;
    value += c - '0';
  }
  return value;
});

// matches a parser enclosed in parentheses: []
template <Parser P>
constexpr auto squares(P&& parser) {
  return between(one_of<'['>, parser, one_of<']'>);
}

// matches a parser enclosed in parentheses: {}
template <Parser P>
constexpr auto brackets(P&& parser) {
  return between(one_of<'{'>, parser, one_of<'}'>);
}

// matches a parser enclosed in parentheses: ()
template <Parser P>
constexpr auto parentheses(P&& parser) {
  return between(one_of<'('>, parser, one_of<')'>);
}

}  // namespace parsec