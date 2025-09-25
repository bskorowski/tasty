#include <functional>
#include <optional>
#include <print>

namespace tasty {

  struct TestInfo {
    constexpr TestInfo(std::string_view testName,
                       std::function<void(void)>&& test)
        : name(testName),
          testFunc(std::move(test)) {}

    std::string name;
    std::function<void(void)> testFunc;
  };

  class TestRunner {
   public:
    constexpr explicit TestRunner(std::string_view suiteName)
        : name(suiteName) {}

    constexpr void registerTest(
        std::function<void()>&& test,
        std::optional<std::string_view> testName = std::nullopt) {
      tests_.emplace_back(testName.value_or(std::to_string(tests_.size() + 1)),
                          std::move(test));
    }

    /**
     * Runs all the registered tests.
     * Tests can be registered with TestRunner::registerTest()
     */
    constexpr auto runAll() -> bool {
      if (tests_.empty()) {
        std::println("No tests to run for test suite: '{}'", name);
      }

      std::println("Running tests for test suite '{}'", name);
      std::size_t passedTests = 0;

      for (const auto& testInfo : tests_) {
        if (runTest(testInfo.testFunc)) {
          ++passedTests;
        }
      }

      if (passedTests == tests_.size()) {
        std::println("All {} tests passed", tests_.size());
      } else {
        std::println("{} tests passed and {} tests failed", passedTests,
                     tests_.size() - passedTests);
      }

      return passedTests == tests_.size();
    }

    constexpr static auto runTest(
        const std::function<void()>& testFunc,
        std::optional<std::string_view> testName = std::nullopt) -> bool {
      bool success = true;
      try {
        std::invoke(testFunc);
        if (testName) {
          std::println("Test '{}' passed", *testName);
        } else {
          std::println("Test passed");
        }

      } catch (const std::exception& ex) {
        if (testName) {
          std::println("Test '{}' failed. Reason: {}", *testName, ex.what());
        } else {
          std::println("Test failed. reason: {}", ex.what());
        }
        success = false;
      }

      return success;
    }

   public:
    std::string_view name;

   private:
    std::vector<TestInfo> tests_;
  };

}  // namespace tasty
