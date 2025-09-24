#pragma once

#include <concepts>
#include <exception>
#include <string>

template <typename T>
concept ExceptionType = std::derived_from<T, std::exception>;

namespace tasty::errors {

  class ExpectFailed : public std::exception {
   public:
    explicit ExpectFailed(std::string&& str)
        : message_(std::move(str)) {}
    auto what() const noexcept -> const char* override {
      return message_.c_str();
    }

   private:
    std::string message_;
  };
}  // namespace tasty::errors
