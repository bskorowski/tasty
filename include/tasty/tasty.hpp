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
  template <std::equality_comparable T>
  constexpr void expectEqual(const T& expected, const T& actual) {
    if (expected != actual) {
      throw errors::ExpectFailed(
          std::format("Expected: {} but got {}", expected, actual));
    }
  }

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
