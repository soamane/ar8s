#ifndef STRING_VALIDATOR_HPP
#define STRING_VALIDATOR_HPP

#include <string_view>

struct StringValidator {
    static const bool IsDigitOnly(std::string_view str);
    static const bool IsRussianPhoneNumber(std::string_view phone);
};

#endif // !STRING_VALIDATOR_HPP