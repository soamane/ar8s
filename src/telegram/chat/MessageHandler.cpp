#include "MessageHandler.hpp"

#include <regex>

bool MessageHandler::IsRussianPhoneNumber(std::string_view message) {
    const std::regex pattern(R"(^(\+7|7|8)?(\s|\-)?\(?(\d{3})\)?(\s|\-)?(\d{3})(\s|\-)?(\d{2})(\s|\-)?(\d{2})$|^(9\d{9})$)");
    return std::regex_match(message.data(), pattern);
}

bool MessageHandler::IsDigitOnly(std::string_view message) {
    std::regex pattern(R"(^\d+$)");
    return std::regex_match(message.data(), pattern);
}

const std::string MessageHandler::NormalizePhoneNumber(std::string_view phone) {
    std::string normalized;

    std::copy_if(phone.begin(), phone.end(), std::back_inserter(normalized), [](char c)
    {
        return std::isdigit(c);
    });

    if (normalized.size() == 11 && (normalized[ 0 ] == '7' || normalized[ 0 ] == '8')) {
        return normalized.substr(1); // Удаляем первую цифру (7 или 8)
    } else if (normalized.size() == 10 && normalized[ 0 ] == '9') {
        return normalized;
    } else {
        return "";
    }
}
