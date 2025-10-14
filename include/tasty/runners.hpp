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

    /**
     * @brief Adds tests to the internal test list
     *
     * The test may be later ran with TestRunner::runAll()
     * @param test The test function to be invoked.
     * @param testName Optional test name. If none provided test number will be
     * used.
     */
    constexpr void registerTest(
        std::function<void()>&& test,
        std::optional<std::string_view> testName = std::nullopt) {
      tests_.emplace_back(((testName) ? std::string(*testName)
                                      : std::to_string(tests_.size() + 1)),
                          std::move(test));
    }

    /**
     * @brief Sets a callback that runs before each test
     *
     * @param callback Callback invoked before each test (when running with
     * runAll)
     */
    constexpr void beforeEach(const std::function<void()>& callback) {
      beforeTestFn_ = callback;
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
        if (beforeTestFn_) {
          (*beforeTestFn_)();
        }

        if (runTest(testInfo.testFunc, testInfo.name)) {
          ++passedTests;
        }
      }

      if (passedTests == tests_.size()) {
        std::println("{}All {} tests passed{}", GREEN, tests_.size(),
                     rainbow::reset());
      } else {
        std::println("{}{} tests passed{} and {}{} tests failed{}", GREEN,
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
          std::println("{}Test failed. reason: {}{}", RED, ex.what(),
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

    std::optional<std::function<void()>> beforeTestFn_ = std::nullopt;
  };

}  // namespace tasty
