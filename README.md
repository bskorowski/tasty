# tasty

A modern, header-only C++ testing library built for C++20/C++23.

> [!CAUTION]
> This project is currently under active development and is not considered finished. the API is definitely a subject to change and is not stable. Features are still being implemented, and there may be unresolved bugs. 

## Features

* **Header-Only:** Drop it into your project and go. No complex build steps or static libraries to link.
* **Test Runners & Lifecycle Hooks:** Group your tests using `tasty::TestRunner` and easily manage state with `beforeEach` and `afterEach` callbacks.
* **Type-Safe Assertions:** Built-in support for checking equality (`expectEqual`), boolean expressions (`TASTY_EXPECT`), and specific exceptions (`expectException`).
* **Modern C++ Diagnostics:** Leverages `<source_location>` for highly precise error reporting, minimizing the reliance on legacy C-style macros.
* **Smart Formatting:** Automatically formats custom types in failure messages if a `std::formatter` exists. C++ symbols and types are automatically demangled for clean, readable output.
* **Colorful Terminal Output:** Powered by `rainbowcpp`, providing clear, color-coded (`MAGENTA`, `GREEN`, `RED`) test results directly in your terminal.

## Requirements

Because `tasty` leverages modern standard library features, you will need a C++20/C++23 compatible compiler:
* GCC, Clang, or MSVC with support for `<concepts>`, `<format>`, `<source_location>`, and `<print>`.

## Installation

Since tasty is header-only, you can simply copy the header files in ```./include/``` directly into your project and use them!

**Using CMake (FetchContent)**
```cmake
include(FetchContent)
FetchContent_Declare(
    tasty
    GIT_REPOSITORY https://github.com/oosiriiss/tasty
    GIT_TAG 0.2.11 # Or your preferred branch/tag/hash
)
FetchContent_Declare(tasty)

# Link the target
target_link_libraries(your_target PRIVATE tasty::tasty)
```

## Quick Start

#### 1. Basic Test Runner

Create a test runner, register your test functions, and run them all with `runAll()`:

```cpp
#include <cstdlib>
#include "tasty/tasty.hpp"

auto main() -> int {
  // Initialize a test suite
  tasty::TestRunner suite("Math Operations");

  // Register a test using a lambda
  suite.registerTest(
      []() {
        int a = 2;
        int b = 2;
        tasty::expectEqual(a + b, 4);
      },
      "Addition Test");

  // Run the suite
  bool success = suite.runAll();
  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
```

#### 2. Setup and Teardown Hooks

If your tests require setting up data before each run, you can utilize the `beforeEach` and `afterEach` hooks. These callbacks are invoked immediately before and after every test in the suite.

```cpp
tasty::TestRunner suite("Database Tests");

suite.beforeEach([]() {
  // Connect to test DB or reset state
});

suite.afterEach([]() {
  // Clean up state
});
```

### Assertions

`tasty` throws structured exceptions (`tasty::errors::ExpectFailed`, `tasty::errors::TestFailed`) internally to halt failing tests and report errors.

Here is a quick reference for the available assertions:

```cpp
// 1. Equality Checks
// Compares using '=='. Prints expected and actual values if they differ.
// Note: Ensure your types have a valid `std::formatter` implemented for 
// the given type as it will be used if available!

tasty::expectEqual(10, calculateScore());

// 2. Boolean Expectations
// Evaluates a boolean expression. Fails and reports exact file/line if false.
// Additionally lets you see the whole expression that failed.
TASTY_EXPECT(user.isValid());

// 3. Exception Testing
// Verifies that a specific exception type is thrown when a function is invoked.
tasty::expectException<std::runtime_error>(parseUser, invalid_json_string);

// 4. Manual Failure
// Immediately fails the current test with an optional custom message.
if (unrecoverableError) {
  tasty::fail("The system entered an invalid state.");
}
```

## Usage

For more detailed usage examples check out [example](./example/) directory.

# License

tasty is licensed under the MIT License. See the [License](./LICENSE) for details.
