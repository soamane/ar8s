#include "MessageHandler.hpp"

#include <regex>

bool MessageHandler::IsRussianPhoneNumber(std::string_view message) {
    const std::regex pattern(R"(^(7|8)\d{10}|^9\d{9}$)");
    return std::regex_match(message.data(), pattern);
}
