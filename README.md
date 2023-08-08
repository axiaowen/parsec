# Parser Combinator
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/axiaowen/parser-combinator/blob/master/LICENSE.MIT)
[![GitHub Actions Status](https://github.com/axiaowen/parser-combinator/workflows/windows/badge.svg?branch=master)](https://github.com/axiaowen/parser-combinator/actions)

An experimental parser combinator library written in C++23. Fully support `constexpr` parsing in compile time.

## Example
```C++
constexpr auto ipv4 = [](
    uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    return (a << 24) | (b << 16) | (c << 8) | d;
};

constexpr auto octet = integer<uint8_t>;
constexpr auto ipv4_addr = eof(sepby<4>(octet, dot));
constexpr auto to_ipv4 = ipv4_addr | to([](auto&& result) {
    auto [a, b, c, d] = result;
    return ipv4( a, b, c, d );
});
static_assert(to_ipv4("192.168.1.1") == ipv4(192, 168, 1, 1));
```

## Monadic
* **to** converts the parser's result to anther type
* **map** maps a function over the result of a parser
* **or_else** applies a function over the input if the parser failed
* **and_then** applies a function over the result of a parser

## Combinator
* **success** consumes no input and always succeeds with given value.
* **predict** return the result of the parser if it satisfies a predictate
* **seq (operator+)** matches a sequence of parsers in the defined order. Return a std::tuple of the two return value of the parsers.
* **choice (operator||)** tries to apply the parsers in order until one of them succeeds.
* **left** matches two parsers and accepts the result from the left side.
* **right** matches two parsers and accepts the result from the right side.
* **between** matches three parsers and accepts the result from the middle one.
* **many** matches a parser multiple times, can be a matched 0 times. Return a std::vector of the return value of the parser.
* **many1** matches a parser multiple times at least one time. Return a std::vector of the return value of the parser.
* **many<>** matches a parser in a fixed amount of times. Return a std::array of the return value of the parser.
* **sepby** matches a parser separated by another parser, can be a matched 0 times.
* **sepby1** matches a parser separated by another parser at least one time.
* **sepby<>** matches a parser separated by another parser in a fixed amount of times.
* **eof** matches the of the input

## Character
* **any** matches any character
* **satisfy** matches one character if it satisfies a predicate.
* **range** matches one character in the range of characters.
* **one_of** matches the character in the list of characters.
* **none_of** matches the character not in the list of characters.
* **digit** matches one numerical character: 0-9.
* **octdigit** matches one octal numerical character: 0-7.
* **hexdigit** matches one hexadecimal numerical character: 0-9, a-f, A-F.
* **lower** matches one lowercase alphabetic character: a-z.
* **upper** matches one uppercase alphabetic character: A-Z.
* **alpha** matches one alphabet character: a-z, A-Z.
* **alphanum** matches one numerical or alphabetic character: 0-9, a-z, A-Z.
* **sign** matches one sign character: -, +.
* **space** matches one whitespace character
* **dot** matches one semi character: ..
* **semi** matches one semi character: ;.
* **comma** matches one comma character: ,.
* **colon** matches one colon character: :.
* **quota** matches one quota character: ".
* **escape** matches one escaped character: \", \\, \/, \b, \f, \n, \r, \t.

## Token
* **symbol** matches a specific string.
* **octal** matches a octal number.
* **decimal** matches a decimal number.
* **hexadecimal** matches a hexadecimal number.
* **squares** matches a parser enclosed in squares: [].
* **brackets** matches a parser enclosed in brackets: {}.
* **parentheses** matches a parser enclosed in parentheses: ().

## Compiler support
* MSVC 19.34+ /std::c++latest