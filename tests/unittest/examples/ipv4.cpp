#include <doctest/doctest.h>
#include <parserc/combinator.h>
#include <parserc/token.h>

using namespace parsec;

TEST_CASE("examples/ipv4") {
  constexpr auto ipv4 = [](uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    return (a << 24) | (b << 16) | (c << 8) | d;
  };

  constexpr auto octet = decimal<uint8_t>;
  constexpr auto ipv4_addr = eof(sepby<4>(octet, dot));
  constexpr auto to_ipv4 = ipv4_addr | to([](auto &&result) {
    auto [a, b, c, d] = result;
    return ipv4(a, b, c, d);
  });
  static_assert(to_ipv4("192.168.1.1") == ipv4(192, 168, 1, 1));

  CHECK(to_ipv4("").has_value() == false);
  CHECK(to_ipv4("1.2.3").has_value() == false);
  CHECK(to_ipv4("1..2.3.4").has_value() == false);
  CHECK(to_ipv4("1.2.3.4.5").has_value() == false);
  CHECK(to_ipv4("0.0.0.0") == ipv4(0, 0, 0, 0));
  CHECK(to_ipv4("192.168.1.1") == ipv4(192, 168, 1, 1));
  CHECK(to_ipv4("255.255.255.255") == ipv4(255, 255, 255, 255));
}