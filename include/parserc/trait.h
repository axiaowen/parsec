#pragma once
#include <concepts>
#include <expected>
#include <optional>
#include <string_view>

namespace parsec {

template <typename R, typename F, typename... Args>
concept invocable_r = std::invocable<F, Args...> &&
                      std::same_as<std::invoke_result_t<F, Args...>, R>;

template <typename T, typename E>
class expected : public std::expected<T, E> {
 public:
  using std::expected<T, E>::expected;

  template <std::invocable<T> F>
  constexpr auto map(F&& f) const& {
    using R = std::invoke_result_t<F, T>;

    if (this->has_value()) {
      return expected<R, E>(std::invoke(std::forward<F>(f), this->value()));
    } else {
      return expected<R, E>(std::unexpect, this->error());
    }
  }

  template <std::invocable<T> F>
  constexpr auto and_then(F&& f) const& {
    using R = std::invoke_result_t<F, T>;

    if (this->has_value()) {
      return std::invoke(std::forward<F>(f), this->value());
    } else {
      return R(std::unexpect, this->error());
    }
  }

  template <std::invocable<> F>
  constexpr auto or_else(F&& f) const& {
    if (this->has_value()) {
      return *this;
    } else {
      return std::invoke(std::forward<F>(f));
    }
  }
};

using Input = std::string_view;
using Error = std::string_view;

template <typename F>
concept Parser = std::invocable<F, Input>;

template <typename T>
using Output = std::tuple<T, Input>;

template <typename T>
using Result = expected<T, Error>;

using ParserError = std::unexpected<Error>;

template <typename T>
using ParserResult = Result<Output<T>>;

template <Parser P>
using invoke_parser_output_t = std::invoke_result_t<P, Input>::value_type;
template <Parser P>
using invoke_parser_result_t =
    std::tuple_element_t<0, invoke_parser_output_t<P>>;

}  // namespace parsec