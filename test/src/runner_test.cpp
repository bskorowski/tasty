#include "tasty/runners.hpp"
#include "tasty/tasty.hpp"

constexpr static void okFunc() { tasty::expectEqual(1, 1); }

constexpr static void failFunc() { tasty::expectEqual(1, 2); }

#define RETURN_ON_FAIL(boolean) /*NOLINT*/ \
  if (!(boolean))                          \
    return -1;

auto main() -> int {  // NOLINT
  const tasty::TestRunner runner("Test Runner");

  RETURN_ON_FAIL(runner.runTest(okFunc));
  RETURN_ON_FAIL(!runner.runTest(failFunc));
}
