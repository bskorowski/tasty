#pragma once

#include <any>
#include <concepts>
#include <exception>
#include <format>
#include <functional>

#include "tasty/errors.hpp"
#include "tasty/tasty_export.hpp"

namespace tasty {

  /**
   * @brief lala
   * @param expected Llaa
   */
  template <typename T>
    requires std::equality_comparable<T>
  constexpr auto expectEqual(const T& expected, const T& actual) -> void {
    if (expected != actual) {
      throw errors::ExpectFailed(
          std::format("Expected: {} but got {}", expected, actual));
    }
  }

  template <ExceptionType Exception, typename Func, typename... Args>
  constexpr auto expectException(Func&& func, Args&&... args) -> void {
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
