#include <print>

#include "tasty/runners.hpp"
#include "tasty/tasty.hpp"

constexpr static void okFunc() { tasty::expectEqual(1, 1); }

auto main() -> int {  // NOLINT
  using tasty::TestRunner;
  // Tests can be ran immediately by passing the test function along with a test
  // number
  TestRunner::runTest(okFunc);
  // Tests may be given names
  TestRunner::runTest([] { tasty::expectEqual(15, 12); },  // NOLINT
                      "This is a failing test");

  // Tests suites may be created
  // Test suites will be ran all at one
  TestRunner suite("Example tests");

  int someGlobalState = 0;

  suite.beforeEach([&someGlobalState]() {
    someGlobalState = 0;
    std::println("Resetting global state to: {}", someGlobalState);
  });

  // Tests may be assigned to test suites like this
  suite.registerTest([&someGlobalState] {
    std::println("Incrementing global state");
    ++someGlobalState;
    std::println("Global state is now: {}", someGlobalState);
  });


  // If no nanme was specifiec for the test in suite it will be automatically
  // given a number
  suite.registerTest(
      [someGlobalState] {
        std::println("Global in second test is: {}", someGlobalState);
      },
      "Custom test name");

  // running all the tests in a suite
  return (suite.runAll()) ? 0 : -1;
}
