#include <functional>
#include <optional>
#include <print>
#include <rainbowcpp/colors.hpp>
#include <rainbowcpp/rainbowcpp.hpp>

constexpr static auto MAGENTA =  // NOLINT
    rainbow::color<rainbow::colors::bit4::Foreground::Magenta,
                   rainbow::colors::bit4::Background::Black>();
constexpr static auto GREEN =  // NOLINT
    rainbow::color<rainbow::colors::bit4::Foreground::Green,
                   rainbow::colors::bit4::Background::Black>();

constexpr static auto RED =  // NOLINT
    rainbow::color<rainbow::colors::bit4::Foreground::Red,
                   rainbow::colors::bit4::Background::Black>();

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

      std::println("{}Running tests for test suite '{}'{}", MAGENTA, name,
                   rainbow::reset());
      std::size_t passedTests = 0;

      for (const auto& testInfo : tests_) {
        if (runTest(testInfo.testFunc)) {
          ++passedTests;
        }
      }

      if (passedTests == tests_.size()) {
        std::println("{}All {} tests passed{}", GREEN, tests_.size(),
                     rainbow::reset());
      } else {
        std::println("{} {}tests passed{} and {} {}tests failed{}", GREEN,
                     passedTests, rainbow::reset(), RED,
                     tests_.size() - passedTests, rainbow::reset());
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
          std::println("{}Test '{}' passed{}", GREEN, *testName,
                       rainbow::reset());
        } else {
          std::println("{}Test passed{}", GREEN, rainbow::reset());
        }

      } catch (const std::exception& ex) {
        if (testName) {
          std::println("{}Test '{}' failed. Reason: {}{}", RED, *testName,
                       ex.what(), rainbow::reset());
        } else {
          std::println("{}Test failed. reason: {}{}", GREEN, ex.what(),
                       rainbow::reset());
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
