#pragma once
#include <algorithm>
#include <ranges>

#include "combinator.h"

namespace parsec {

// matches any char
constexpr auto any() {
  using R = ParserResult<char>;
  return [](const Input& input) {
    if (input.empty()) {
      return R{ std::unexpect, "input is empty" };
    }
    return R{ { input.at(0), input.substr(1) } };
  };
};

template <class F>
constexpr auto satisfy(F&& f) {
  return predict(any(), f);
}

template <char begin, char end>
constexpr auto range = satisfy([](char c) {
  return begin <= c && c <= end;
});

template <char... chs>
constexpr auto one_of = satisfy([](char c) {
  constexpr char arr[] = { chs... };
  return std::ranges::contains(arr, c);
});

template <char... chs>
constexpr auto none_of = satisfy([](char c) {
  constexpr char arr[] = { chs... };
  return !std::ranges::contains(arr, c);
});

constexpr auto digit = range<'0', '9'>;
constexpr auto octdigit = range<'0', '7'>;
constexpr auto hexdigit = range<'0', '9'> || range<'A', 'F'> || range<'a', 'f'>;
constexpr auto lower = range<'a', 'z'>;
constexpr auto upper = range<'A', 'Z'>;
constexpr auto alpha = lower || upper;
constexpr auto alphanum = alpha || digit;
constexpr auto sign = one_of<'+', '-'>;
constexpr auto space = one_of<' ', '\n', '\r', '\t'>;
constexpr auto dot = one_of<'.'>;
constexpr auto semi = one_of<';'>;
constexpr auto comma = one_of<','>;
constexpr auto colon = one_of<':'>;
constexpr auto quota = one_of<'\"'>;

}  // namespace parsec