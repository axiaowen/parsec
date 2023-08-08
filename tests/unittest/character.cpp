#include <doctest/doctest.h>
#include <parserc/character.h>

using namespace parsec;

TEST_CASE("range") {
  CHECK(range<'a', 'd'>("e").has_value() == false);
  CHECK(range<'a', 'd'>("a") == std::make_tuple('a', ""));
  CHECK(range<'a', 'd'>("b") == std::make_tuple('b', ""));
  CHECK(range<'a', 'd'>("c") == std::make_tuple('c', ""));
  CHECK(range<'a', 'd'>("d") == std::make_tuple('d', ""));

  static_assert(range<'a', 'd'>("e").has_value() == false);
  static_assert(range<'a', 'd'>("a") == std::make_tuple('a', ""));
  static_assert(range<'a', 'd'>("b") == std::make_tuple('b', ""));
  static_assert(range<'a', 'd'>("c") == std::make_tuple('c', ""));
  static_assert(range<'a', 'd'>("d") == std::make_tuple('d', ""));
}

TEST_CASE("one_of") {
  CHECK(one_of<'a'>("").has_value() == false);
  CHECK(one_of<'a'>("b").has_value() == false);
  CHECK(one_of<'a'>("a") == std::make_tuple('a', ""));
  CHECK(one_of<'a'>("ab") == std::make_tuple('a', "b"));

  CHECK(one_of<'a', 'b', 'c'>("").has_value() == false);
  CHECK(one_of<'a', 'b', 'c'>("d").has_value() == false);
  CHECK(one_of<'a', 'b', 'c'>("a") == std::make_tuple('a', ""));
  CHECK(one_of<'a', 'b', 'c'>("b") == std::make_tuple('b', ""));
  CHECK(one_of<'a', 'b', 'c'>("c") == std::make_tuple('c', ""));
  CHECK(one_of<'a', 'b', 'c'>("ab") == std::make_tuple('a', "b"));

  static_assert(one_of<'a'>("").has_value() == false);
  static_assert(one_of<'a'>("b").has_value() == false);
  static_assert(one_of<'a'>("a") == std::make_tuple('a', ""));
  static_assert(one_of<'a'>("ab") == std::make_tuple('a', "b"));

  static_assert(one_of<'a', 'b', 'c'>("").has_value() == false);
  static_assert(one_of<'a', 'b', 'c'>("d").has_value() == false);
  static_assert(one_of<'a', 'b', 'c'>("a") == std::make_tuple('a', ""));
  static_assert(one_of<'a', 'b', 'c'>("b") == std::make_tuple('b', ""));
  static_assert(one_of<'a', 'b', 'c'>("c") == std::make_tuple('c', ""));
  static_assert(one_of<'a', 'b', 'c'>("ab") == std::make_tuple('a', "b"));
}

TEST_CASE("none_of") {
  CHECK(none_of<'a'>("").has_value() == false);
  CHECK(none_of<'a'>("a").has_value() == false);
  CHECK(none_of<'a'>("b") == std::make_tuple('b', ""));
  CHECK(none_of<'a'>("ca") == std::make_tuple('c', "a"));

  CHECK(none_of<'a', 'b', 'c'>("").has_value() == false);
  CHECK(none_of<'a', 'b', 'c'>("a").has_value() == false);
  CHECK(none_of<'a', 'b', 'c'>("b").has_value() == false);
  CHECK(none_of<'a', 'b', 'c'>("c").has_value() == false);
  CHECK(none_of<'a', 'b', 'c'>("d") == std::make_tuple('d', ""));
  CHECK(none_of<'a', 'b', 'c'>("ef") == std::make_tuple('e', "f"));

  static_assert(none_of<'a'>("").has_value() == false);
  static_assert(none_of<'a'>("a").has_value() == false);
  static_assert(none_of<'a'>("b") == std::make_tuple('b', ""));
  static_assert(none_of<'a'>("ca") == std::make_tuple('c', "a"));

  static_assert(none_of<'a', 'b', 'c'>("").has_value() == false);
  static_assert(none_of<'a', 'b', 'c'>("a").has_value() == false);
  static_assert(none_of<'a', 'b', 'c'>("b").has_value() == false);
  static_assert(none_of<'a', 'b', 'c'>("c").has_value() == false);
  static_assert(none_of<'a', 'b', 'c'>("d") == std::make_tuple('d', ""));
  static_assert(none_of<'a', 'b', 'c'>("ef") == std::make_tuple('e', "f"));
}

TEST_CASE("character") {
  CHECK(digit("a").has_value() == false);
  CHECK(digit("1") == std::make_tuple('1', ""));
  CHECK(octdigit("8").has_value() == false);
  CHECK(octdigit("1") == std::make_tuple('1', ""));
  CHECK(hexdigit("x").has_value() == false);
  CHECK(hexdigit("1") == std::make_tuple('1', ""));
  CHECK(hexdigit("a") == std::make_tuple('a', ""));
  CHECK(lower("A").has_value() == false);
  CHECK(lower("a") == std::make_tuple('a', ""));
  CHECK(upper("a").has_value() == false);
  CHECK(upper("A") == std::make_tuple('A', ""));
  CHECK(alpha("0").has_value() == false);
  CHECK(alpha("a") == std::make_tuple('a', ""));
  CHECK(alpha("A") == std::make_tuple('A', ""));
  CHECK(alphanum("1") == std::make_tuple('1', ""));
  CHECK(alphanum("a") == std::make_tuple('a', ""));
  CHECK(alphanum("A") == std::make_tuple('A', ""));
  CHECK(sign("-") == std::make_tuple('-', ""));
  CHECK(sign("+") == std::make_tuple('+', ""));
  CHECK(space(" ") == std::make_tuple(' ', ""));
  CHECK(space("\n") == std::make_tuple('\n', ""));
  CHECK(dot(".") == std::make_tuple('.', ""));
  CHECK(semi(";") == std::make_tuple(';', ""));
  CHECK(comma(",") == std::make_tuple(',', ""));
  CHECK(colon(":") == std::make_tuple(':', ""));
  CHECK(quota("\"") == std::make_tuple('"', ""));

  static_assert(digit("a").has_value() == false);
  static_assert(digit("1") == std::make_tuple('1', ""));
  static_assert(octdigit("8").has_value() == false);
  static_assert(octdigit("1") == std::make_tuple('1', ""));
  static_assert(hexdigit("x").has_value() == false);
  static_assert(hexdigit("1") == std::make_tuple('1', ""));
  static_assert(hexdigit("a") == std::make_tuple('a', ""));
  static_assert(lower("A").has_value() == false);
  static_assert(lower("a") == std::make_tuple('a', ""));
  static_assert(upper("a").has_value() == false);
  static_assert(upper("A") == std::make_tuple('A', ""));
  static_assert(alpha("0").has_value() == false);
  static_assert(alpha("a") == std::make_tuple('a', ""));
  static_assert(alpha("A") == std::make_tuple('A', ""));
  static_assert(alphanum("1") == std::make_tuple('1', ""));
  static_assert(alphanum("a") == std::make_tuple('a', ""));
  static_assert(alphanum("A") == std::make_tuple('A', ""));
  static_assert(sign("-") == std::make_tuple('-', ""));
  static_assert(sign("+") == std::make_tuple('+', ""));
  static_assert(space(" ") == std::make_tuple(' ', ""));
  static_assert(space("\n") == std::make_tuple('\n', ""));
  static_assert(dot(".") == std::make_tuple('.', ""));
  static_assert(semi(";") == std::make_tuple(';', ""));
  static_assert(comma(",") == std::make_tuple(',', ""));
  static_assert(colon(":") == std::make_tuple(':', ""));
  static_assert(quota("\"") == std::make_tuple('"', ""));
}