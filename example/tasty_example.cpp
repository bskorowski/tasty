#include <format>
#include <print>
#include <stdexcept>
#include <string_view>

#include "tasty/errors.hpp"
#include "tasty/tasty.hpp"

static constexpr auto throwsIfTrue(
    bool shouldThrow, [[maybe_unused]] std::string_view someOtherArg) -> void {
  if (shouldThrow) {
    throw std::invalid_argument(std::format(
        "If {} == true thne this exception is expected", shouldThrow));
  }
}

struct UnformattableStruct {
  int x;
  int y;

  auto operator==(const UnformattableStruct& other) const -> bool {
    return x == other.x && y == other.y;
  }
};

struct UnformattableStructWithFormatter {
  int x;
  int z;

  auto operator==(const UnformattableStructWithFormatter& other) const -> bool {
    return x == other.x && z == other.z;
  }
};

namespace std {
  template <>
  struct formatter<UnformattableStructWithFormatter, char> {
    static constexpr auto parse(std::format_parse_context& ctx) {
      return ctx.begin();
    }

    static auto format(const UnformattableStructWithFormatter& strct,
                       std::format_context& ctx) {
      return std::format_to(ctx.out(),
                            "UnformattableStructWithFormatter(x={}, y={})",
                            strct.x, strct.z);
    }
  };
}  // namespace std

constexpr auto throwNonExcpetion() { throw 15; }  // NOLINT

auto main() -> int {  // NOLINT
  // If you want to assert that two values are equal you can use
  // tasty::expectEqual(T expected, T actual). The function requires the T type
  // to be std::equality_comparable.
  // If the values don't match it will throw and tasty:errors::ExpectFailed
  // exception

  constexpr int num11 = 32;
  constexpr int num12 = 15;

  try {
    tasty::expectEqual(num11, num12);
  } catch (const tasty::errors::ExpectFailed& err) {
    std::println("expectEqual doesn't like the expression: {} == {}", num11,
                 num12);
  }

  // Passing two values that match will not throw
  constexpr int num21 = 69;
  constexpr int num22 = 69;
  tasty::expectEqual(num21, num22);
  std::println("expectEqual likes the expression: {} == {}", num21, num22);

  // You can also expect a specific exception to be thrown by passing it as a
  // template parameter. In that case you can pass a std::invocable object along
  // with it's arguments that will be called and if it throws a different
  // exception, different value or doesn't throw at all
  // tasty::errors::ExpectFailed will be thrown
  tasty::expectException<std::invalid_argument>(throwsIfTrue, true,
                                                "Hello im an argument");
  std::println(
      "Function threw std::invalid_argument exception which was expected");

  try {
    tasty::expectException<std::runtime_error>(throwsIfTrue, true,
                                               "Hello im an argument");
  } catch (const tasty::errors::ExpectFailed& err) {
    std::println(
        "Function threw exception different than expected std::runtime_error");
  }

  try {
    tasty::expectException<std::runtime_error>(throwNonExcpetion);
  } catch (const tasty::errors::ExpectFailed& err) {
    std::println(
        "Function threw something other than expected std::runtime_error "
        "exception");
  }

  // If a type without std::formatter specialization is encountered tasty cannot
  // format it correctly, so it will just throw an error that expect failed
  // (because values don't match), And no  // detailed value information will be
  // provided since tasty cannot print it. (?yet?)
  try {
    tasty::expectEqual(UnformattableStruct{.x = 5, .y = 9},        // NOLINT
                       UnformattableStruct{.x = 152, .y = 1952});  // NOLINT
  } catch (const tasty::errors::ExpectFailed& err) {
    std::println("{}", err.what());
  }

  // If a std::formatter is provided it is printed nicely
  try {
    tasty::expectEqual(
        UnformattableStructWithFormatter{.x = 5, .z = 9},        // NOLINT
        UnformattableStructWithFormatter{.x = 152, .z = 1952});  // NOLINT
  } catch (const tasty::errors::ExpectFailed& err) {
    std::println("{}", err.what());
  }
}
