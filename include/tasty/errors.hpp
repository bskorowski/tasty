#include <exception>

namespace tasty::errors {
  class ExpectFailed : public std::exception {};

}  // namespace tasty::errors
