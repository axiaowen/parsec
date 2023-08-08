#include <doctest/doctest.h>
#include <parserc/character.h>
#include <parserc/combinator.h>

using namespace parsec;

TEST_CASE("seq") {
  constexpr auto parse = one_of<'a'> + one_of<'b'>;
  constexpr auto value = std::make_tuple('a', 'b');

  static_assert(parse("ba").has_value() == false);
  static_assert(parse("ab") == std::make_tuple(value, ""));
  static_assert(parse("abc") == std::make_tuple(value, "c"));

  CHECK(parse("ba").has_value() == false);
  CHECK(parse("ab") == std::make_tuple(value, ""));
  CHECK(parse("abc") == std::make_tuple(value, "c"));
}

TEST_CASE("choice") {
  constexpr auto parse = one_of<'a'> || one_of<'b'>;

  static_assert(parse("c").has_value() == false);
  static_assert(parse("a") == std::make_tuple('a', ""));
  static_assert(parse("b") == std::make_tuple('b', ""));
  static_assert(parse("ab") == std::make_tuple('a', "b"));

  CHECK(parse("c").has_value() == false);
  CHECK(parse("a") == std::make_tuple('a', ""));
  CHECK(parse("b") == std::make_tuple('b', ""));
  CHECK(parse("ab") == std::make_tuple('a', "b"));
}

TEST_CASE("left") {
  constexpr auto parse = left(one_of<'a'>, one_of<'b'>);

  static_assert(parse("ba").has_value() == false);
  static_assert(parse("ab") == std::make_tuple('a', ""));
  static_assert(parse("abc") == std::make_tuple('a', "c"));

  CHECK(parse("ba").has_value() == false);
  CHECK(parse("ab") == std::make_tuple('a', ""));
  CHECK(parse("abc") == std::make_tuple('a', "c"));
}

TEST_CASE("right") {
  constexpr auto parse = right(one_of<'a'>, one_of<'b'>);

  static_assert(parse("ba").has_value() == false);
  static_assert(parse("ab") == std::make_tuple('b', ""));
  static_assert(parse("abc") == std::make_tuple('b', "c"));

  CHECK(parse("ba").has_value() == false);
  CHECK(parse("ab") == std::make_tuple('b', ""));
  CHECK(parse("abc") == std::make_tuple('b', "c"));
}

TEST_CASE("between") {
  constexpr auto parse = between(one_of<'a'>, one_of<'b'>, one_of<'c'>);

  static_assert(parse("bac").has_value() == false);
  static_assert(parse("abc") == std::make_tuple('b', ""));
  static_assert(parse("abcd") == std::make_tuple('b', "d"));

  CHECK(parse("bac").has_value() == false);
  CHECK(parse("abc") == std::make_tuple('b', ""));
  CHECK(parse("abcd") == std::make_tuple('b', "d"));
}

TEST_CASE("many") {
  constexpr auto parse = many(one_of<'a'>);
  constexpr auto value = [](std::size_t count) {
    return std::vector<char>(count, 'a');
  };

  static_assert(parse("") == std::make_tuple(value(0), ""));
  static_assert(parse("b") == std::make_tuple(value(0), "b"));
  static_assert(parse("a") == std::make_tuple(value(1), ""));
  static_assert(parse("aaa") == std::make_tuple(value(3), ""));
  static_assert(parse("aaab") == std::make_tuple(value(3), "b"));

  CHECK(parse("") == std::make_tuple(value(0), ""));
  CHECK(parse("b") == std::make_tuple(value(0), "b"));
  CHECK(parse("a") == std::make_tuple(value(1), ""));
  CHECK(parse("aaa") == std::make_tuple(value(3), ""));
  CHECK(parse("aaab") == std::make_tuple(value(3), "b"));
}

TEST_CASE("many1") {
  constexpr auto parse = many1(one_of<'a'>);
  constexpr auto value = [](std::size_t count) {
    return std::vector<char>(count, 'a');
  };

  static_assert(parse("").has_value() == false);
  static_assert(parse("b").has_value() == false);
  static_assert(parse("a") == std::make_tuple(value(1), ""));
  static_assert(parse("aaa") == std::make_tuple(value(3), ""));
  static_assert(parse("aaab") == std::make_tuple(value(3), "b"));

  CHECK(parse("").has_value() == false);
  CHECK(parse("b").has_value() == false);
  CHECK(parse("a") == std::make_tuple(value(1), ""));
  CHECK(parse("aaa") == std::make_tuple(value(3), ""));
  CHECK(parse("aaab") == std::make_tuple(value(3), "b"));
}

TEST_CASE("many<N>") {
  constexpr auto parse = many<3>(one_of<'a'>);
  constexpr auto value = std::array<char, 3>{ 'a', 'a', 'a' };

  static_assert(parse("aa").has_value() == false);
  static_assert(parse("aaa") == std::make_tuple(value, ""));
  static_assert(parse("aaaa") == std::make_tuple(value, "a"));
  static_assert(parse("aaab") == std::make_tuple(value, "b"));

  CHECK(parse("aa").has_value() == false);
  CHECK(parse("aaa") == std::make_tuple(value, ""));
  CHECK(parse("aaaa") == std::make_tuple(value, "a"));
  CHECK(parse("aaab") == std::make_tuple(value, "b"));
}

TEST_CASE("sepby") {
  constexpr auto parse = sepby(one_of<'a'>, one_of<','>);
  constexpr auto value = [](std::size_t count) {
    return std::vector<char>(count, 'a');
  };

  static_assert(parse("") == std::make_tuple(value(0), ""));
  static_assert(parse("b") == std::make_tuple(value(0), "b"));
  static_assert(parse("a") == std::make_tuple(value(1), ""));
  static_assert(parse("a,") == std::make_tuple(value(1), ","));
  static_assert(parse("a,b") == std::make_tuple(value(1), ",b"));
  static_assert(parse("a,a,a") == std::make_tuple(value(3), ""));

  CHECK(parse("") == std::make_tuple(value(0), ""));
  CHECK(parse("b") == std::make_tuple(value(0), "b"));
  CHECK(parse("a") == std::make_tuple(value(1), ""));
  CHECK(parse("a,") == std::make_tuple(value(1), ","));
  CHECK(parse("a,b") == std::make_tuple(value(1), ",b"));
  CHECK(parse("a,a,a") == std::make_tuple(value(3), ""));
}

TEST_CASE("sepby1") {
  constexpr auto parse = sepby1(one_of<'a'>, one_of<','>);
  constexpr auto value = [](std::size_t count) {
    return std::vector<char>(count, 'a');
  };

  static_assert(parse("").has_value() == false);
  static_assert(parse("b").has_value() == false);
  static_assert(parse("a") == std::make_tuple(value(1), ""));
  static_assert(parse("a,") == std::make_tuple(value(1), ","));
  static_assert(parse("a,b") == std::make_tuple(value(1), ",b"));
  static_assert(parse("a,a,a") == std::make_tuple(value(3), ""));

  CHECK(parse("").has_value() == false);
  CHECK(parse("b").has_value() == false);
  CHECK(parse("a") == std::make_tuple(value(1), ""));
  CHECK(parse("a,") == std::make_tuple(value(1), ","));
  CHECK(parse("a,b") == std::make_tuple(value(1), ",b"));
  CHECK(parse("a,a,a") == std::make_tuple(value(3), ""));
  CHECK(parse("a,a,a") == std::make_tuple(value(3), ""));
}

TEST_CASE("sepby<N>") {
  constexpr auto parse = sepby<3>(one_of<'a'>, one_of<','>);
  constexpr auto value = std::array<char, 3>{ 'a', 'a', 'a' };

  static_assert(parse("a,a").has_value() == false);
  static_assert(parse("a,a,a") == std::make_tuple(value, ""));
  static_assert(parse("a,a,a,a") == std::make_tuple(value, ",a"));
  static_assert(parse("a,a,a,b") == std::make_tuple(value, ",b"));

  CHECK(parse("a,a").has_value() == false);
  CHECK(parse("a,a,a") == std::make_tuple(value, ""));
  CHECK(parse("a,a,a,a") == std::make_tuple(value, ",a"));
  CHECK(parse("a,a,a,b") == std::make_tuple(value, ",b"));
}

TEST_CASE("eof") {
  constexpr auto parse = eof(one_of<'a'>);

  static_assert(parse("ab").has_value() == false);
  static_assert(parse("a") == std::make_tuple('a', ""));

  CHECK(parse("ab").has_value() == false);
  CHECK(parse("a") == std::make_tuple('a', ""));
}