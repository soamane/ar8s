#include "input_validator.hpp"

#include <regex>

bool InputValidator::IsDigitOnly(std::string_view str) {
    const std::regex pattern(R"(^\d+$)");
    return std::regex_match(str.data(), pattern);
}

bool InputValidator::IsRussianPhoneNumber(std::string_view str) {
    const std::regex pattern(R"(^(\+7|7|8)?(\s|-)?\(?(\d{3})\)?(\s|-)?(\d{3})(\s|-)?(\d{2})(\s|-)?(\d{2})$|^(9\d{9})$)");
    return std::regex_match(str.data(), pattern);
}
