#include <tasty/runners.hpp>
#include <tasty/tasty.hpp>

auto main() -> int {
  auto runner = tasty::TestRunner("Utility tests");

  runner.registerTest(
      []() {
        auto zero = tasty::internal::toString(0);
        auto minusOne = tasty::internal::toString(-1);
        auto thousand = tasty::internal::toString(1000);  // NOLINT

        tasty::expectEqual("0", zero);
        tasty::expectEqual("-1", minusOne);
        tasty::expectEqual("1000", thousand);
      },
      "To string tests");

  runner.registerTest(
      []() {
        tasty::expectEqual(1UL, tasty::internal::digits(0UL));
        tasty::expectEqual(1UL, tasty::internal::digits(1UL));
        tasty::expectEqual(1UL, tasty::internal::digits(9UL));     // NOLINT
        tasty::expectEqual(2UL, tasty::internal::digits(10UL));    // NOLINT
        tasty::expectEqual(3UL, tasty::internal::digits(100UL));   // NOLINT
        tasty::expectEqual(4UL, tasty::internal::digits(1000UL));  // NOLINT
      },
      "Digit count test");

  if (!runner.runAll()) {
    return -1;
  }
}
