#pragma once

#include <concepts>
#include <exception>
#include <filesystem>
#include <format>
#include <functional>
#include <print>
#include <source_location>
#include <type_traits>

#include "tasty/errors.hpp"
#include "tasty/tasty_export.hpp"

#ifdef _MSC_VER

#else
#include <cxxabi.h>
#endif

namespace tasty {

  namespace internal {

    template <typename T, typename CharT>
    concept isFormattable = requires(T& val, std::format_context ctx) {
      std::formatter<std::remove_cvref_t<T>, CharT>().format(val, ctx);
    };

#ifdef _MSC_VER
    template <typename T>
    constexpr auto typeName() -> std::string {
      return typeid(T).name();
    }
#else
    template <typename T>
    constexpr auto typeName() -> std::string {
      int status = -1;
      auto demangledName = std::string(
          abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status));
      if (status != 0) {
        return "Unknown";
      }
      return demangledName;
    }
#endif

    [[nodiscard]] constexpr auto formatSourceLocation(
        const std::source_location& sourceLocation) -> std::string {
      std::string_view fileName = sourceLocation.file_name();

      // Cutting out the path before file name.
      if (const std::size_t index =
              fileName.find_last_of(std::filesystem::path::preferred_separator);
          index != std::string_view::npos) {
        fileName.remove_prefix(index + 1);
      }

      return std::format("{}:{}", fileName, sourceLocation.line());
    }

  }  // namespace internal

#define TASTY_EXPECT(expression)                               \
  if (!(expression)) {                                         \
    throw tasty::errors::ExpectFailed(                         \
        std::format("{} | Expression '{}' evaluated to false", \
                    tasty::internal::formatSourceLocation(   \
                        std::source_location::current()),      \
                    #expression));                             \
  }

  /** @brief lala
   * @param expected Llaa
   */
  template <typename T, typename U>
    requires std::equality_comparable_with<T, U>
  constexpr void expectEqual(
      const T& expected, const U& actual,
      std::source_location sourceLoc = std::source_location::current()) {
    if (expected != actual) {
      if constexpr (internal::isFormattable<T, char>) {
        throw errors::ExpectFailed(std::format(
            "{} | Expected: {} but got {}",
            internal::formatSourceLocation(sourceLoc), expected, actual));
      } else {
        throw errors::ExpectFailed(std::format(
            "{} | Unexpected value encountered. No std::formatter for "
            "type '{}' exists. Cannot print it.",
            internal::formatSourceLocation(sourceLoc),
            internal::typeName<T>()));
      }
    }
  }

  // Couldn't get it to work with std::source_location yet
  template <ExceptionType Exception, typename Func, typename... Args>
    requires std::invocable<Func, Args...>
  constexpr void expectException(Func&& func, Args&&... args) {
    try {
      std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);

      throw errors::ExpectFailed("Function didn't throw any exception");

    } catch (const Exception& targetException) {
      // OK
      return;
    } catch (const std::exception& other) {
      throw errors::ExpectFailed(std::format(
          "Function threw invalid exception. exc.what()={}", other.what()));
    } catch (...) {
      throw errors::ExpectFailed("Function threw invalid non-std::exception.");
    }
  }

}  // namespace tasty
