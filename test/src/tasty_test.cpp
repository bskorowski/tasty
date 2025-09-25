#include "tasty/tasty.hpp"

#include <format>
#include <stdexcept>

static constexpr auto throwsIfTrue(bool shouldThrow, int someOtherArg,
                                   float someNextArg) -> int {
  if (shouldThrow) {
    throw std::invalid_argument(std::format(
        "If {} == true thne this exception is expected", shouldThrow));
  }

  const int lala = someOtherArg + static_cast<int>(someNextArg);
  return lala;
}

#define SHOULD_THROW(x) /*NOLINT*/ \
  try {                            \
    x;                             \
    return -1;                     \
  } catch (...) {                  \
  }
#define SHOULD_NOT_THROW(x) /*NOLINT*/ \
  try {                                \
    x;                                 \
  } catch (...) {                      \
    return -1;                         \
  }

constexpr auto throwNonExcpetion() { throw 15; }  // NOLINT

auto main() -> int {
  SHOULD_THROW(tasty::expectEqual(32, 15);)
  SHOULD_NOT_THROW(tasty::expectEqual(69, 69));
  SHOULD_NOT_THROW(
      tasty::expectException<std::invalid_argument>(throwsIfTrue, true,
                                                    15,      // NOLINT
                                                    10.0F);  // NOLINT
  );
  SHOULD_THROW(tasty::expectException<std::runtime_error>(throwsIfTrue, true,
                                                          15,      // NOLINT
                                                          15.0F);  // NOLINT
  );
  SHOULD_THROW(tasty::expectException<std::runtime_error>(throwNonExcpetion));
}
