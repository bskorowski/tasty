
#include "tasty/errors.hpp"
#include "tasty/runners.hpp"
#include "tasty/tasty.hpp"

constexpr static void okFunc() { tasty::expectEqual(1, 1); }

constexpr static void failFunc() { tasty::expectEqual(1, 2); }

#define RETURN_ON_FAIL(boolean) /*NOLINT*/ \
  if (!(boolean))                          \
    return -1;

auto main() -> int {  // NOLINT
  tasty::TestRunner runner("Test Runner");

  RETURN_ON_FAIL(runner.runTest(okFunc));
  // RETURN_ON_FAIL(!runner.runTest(failFunc));

  int x = 0;

  bool success = false;

  runner.registerTest([&x]() { tasty::expectEqual(x, 1); });
  runner.beforeEach([&x]() { x = 1; });
  runner.afterEach([&x]() { x = 2; });
  success = runner.runAll();
  tasty::expectEqual(x, 2);

  if (!success) {
    return -1;
  }
  return 0;
}
