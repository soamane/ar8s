#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include <string>
#include <string_view>

struct MessageHandler {
public:
    static bool IsRussianPhoneNumber(std::string_view message);
    static bool IsDigitOnly(std::string_view message);
    static const std::string NormalizePhoneNumber(std::string_view phone);
};

#endif // !MESSAGE_HANDLER_HPP