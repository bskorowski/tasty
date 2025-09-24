#pragma once

#include <concepts>

#include "tasty/errors.hpp"
#include "tasty/tasty_export.hpp"

namespace tasty {

  template <typename T>
  constexpr auto expectEqual(T expected, T equal) -> void {
    if (expected != equal) {
      throw errors::ExpectFailed();
    }
  }

}  // namespace tasty
