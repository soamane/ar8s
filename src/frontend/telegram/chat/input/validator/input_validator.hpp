#ifndef INPUT_VALIDATOR_HPP
#define INPUT_VALIDATOR_HPP

#include <string_view>

namespace InputValidator {
    const bool IsDigitOnly(std::string_view str);
    const bool IsRussianPhoneNumber(std::string_view str);
}

#endif // !INPUT_VALIDATOR_HPP