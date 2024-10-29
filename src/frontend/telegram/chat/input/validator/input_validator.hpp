#ifndef INPUT_VALIDATOR_HPP
#define INPUT_VALIDATOR_HPP

#include <string_view>

namespace InputValidator {
    bool IsDigitOnly(std::string_view str);
    bool IsRussianPhoneNumber(std::string_view str);
}

#endif // INPUT_VALIDATOR_HPP
