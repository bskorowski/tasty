#pragma once
#include <concepts>
#include <functional>
#include <optional>
#include <print>
#include <rainbowcpp/colors.hpp>
#include <rainbowcpp/rainbowcpp.hpp>
#include <type_traits>

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

  namespace internal {
    struct TestInfo {
      constexpr TestInfo(std::string_view testName,
                         std::function<void(void)>&& test)
          : name(testName),
            testFunc(std::move(test)) {}

      std::string name;
      std::function<void(void)> testFunc;
    };

    template <std::unsigned_integral T>
    [[nodiscard]] constexpr auto digits(T num, const std::uint8_t base = 10)
        -> std::size_t {
      std::size_t count = 1;
      while (num > 9) {  // NOLINT
        num = num / base;
        ++count;
      }
      return count;
    }

    template <std::integral T>
    [[nodiscard]] constexpr auto toString(T num, const std::uint8_t base = 10)
        -> std::string {
      auto current = static_cast<std::size_t>((num > 0) ? num : -num);
      std::size_t digitCount = internal::digits(current);
      std::size_t requiredChars = (num < 0) ? (digitCount + 1) : digitCount;
      std::string str(requiredChars, '-');

      for (std::size_t i = 0; i < digitCount; ++i) {
        auto digit = static_cast<char>('0' + (current % base));
        str[str.size() - 1 - i] = digit;
        current = current / base;
      }

      return str;
    }

  }  // namespace internal

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
                                      : internal::toString(tests_.size() + 1)),
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
    std::vector<internal::TestInfo> tests_;

    std::optional<std::function<void()>> beforeTestFn_ = std::nullopt;
  };

}  // namespace tasty
