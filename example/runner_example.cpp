#include "tasty/runners.hpp"
#include "tasty/tasty.hpp"

constexpr static void okFunc() { tasty::expectEqual(1, 1); }

auto main() -> int {  // NOLINT
  using tasty::TestRunner;
  // Tests can be ran immediately by passing the test function along with a test
  // number
  TestRunner::runTest(okFunc);
  // Tests may be given names
  TestRunner::runTest([] {}, "This is a failing test");

  // Tests suites may be created
  // Test suites will be ran all at one
  TestRunner suite("Example tests");

  // Tests may be assigned to test suites like this
  suite.registerTest([] { return "a test:)"; });

  // If no nanme was specifiec for the test in suite it will be automatically
  // given a number
  suite.registerTest([] { return "yet another  test:)"; }, "Custom test name");

  // running all the tests in a suite
  return (suite.runAll()) ? 0 : -1;
}
