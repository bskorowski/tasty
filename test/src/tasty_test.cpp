#include "tasty/tasty.hpp"

#include <print>
#include <stdexcept>
#include <string>
#include <format>

constexpr auto throwsIfTrue(bool shouldThrow, int someOtherArg,
                            float someNextArg) -> int {
  if (shouldThrow) {
    throw std::invalid_argument(std::format(
        "If {} == true thne this exception is expected", shouldThrow));
  }

  int lala = someOtherArg + static_cast<int>(someNextArg);
  return lala;
}

constexpr auto throwNonExcpetion() { throw 15; }  // NOLINT

auto main() -> int {
  // function correctly asserts two varialbes are equal
  {
    constexpr int num1 = 32;
    constexpr int num2 = 15;

    // This should throw
    try {
      tasty::expectEqual(num1, num2);
      std::println("expectEqual likes {} == {} (which is bad)", num1, num2);
    } catch (const tasty::errors::ExpectFailed& err) {
      std::println("expectEqual doesnt like {} == {} (which is good)", num1,
                   num2);
    }
  }

  // function correctly asserts two varialbes are equal
  {
    constexpr int num1 = 69;
    constexpr int num2 = 69;
    try {
      tasty::expectEqual(num1, num1);
      std::println("expectEqual likes {} == {} (which is good)", num1, num2);
    } catch (const tasty::errors::ExpectFailed& err) {
      std::println("expectEqual doesnt like {} == {} (which is bad)", num1,
                   num2);
    }
  }

  // Function throws good exception
  try {
    tasty::expectException<std::invalid_argument>(throwsIfTrue, true, 15,
                                                  10.0F);
    std::println("Function threw expected exception (which is good)");
  } catch (const tasty::errors::ExpectFailed& err) {
    std::println("error: {}", err.what());
  }

  // Function throws unexpected  exception
  try {
    tasty::expectException<std::runtime_error>(throwsIfTrue, true, 15, 15.0f);
    std::println("Function threw expected exception (which is bad)");
  } catch (const tasty::errors::ExpectFailed& err) {
    std::println("Function threw unexpected exception (which is good)");
  }

  // function throws non-std::exception
  try {
    tasty::expectException<std::runtime_error>(throwNonExcpetion);
    std::println("Function threw expected exception (which is bad)");
  } catch (const tasty::errors::ExpectFailed& err) {
    std::println("{}",err.what());
  }
}
