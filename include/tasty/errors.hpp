#pragma once

#include <concepts>
#include <exception>
#include <optional>
#include <string>
#include <utility>

template <typename T>
concept ExceptionType = std::derived_from<T, std::exception>;

namespace tasty::errors {

  class ExpectFailed : public std::exception {
   public:
    constexpr explicit ExpectFailed(std::string message)
        : message_(std::move(message)) {}
    constexpr auto what() const noexcept -> const char* override {
      return message_.c_str();
    }

   private:
    std::string message_;
  };

  class TestFailed : public std::exception {
   public:
    constexpr explicit TestFailed(std::optional<std::string> message)
        : message_(std::move(message)) {}

    constexpr auto what() const noexcept -> const char* override {
      if (message_.has_value()) {
        return message_->c_str();
      }
      return "No reason";
    }

   private:
    std::optional<std::string> message_;
  };

}  // namespace tasty::errors
