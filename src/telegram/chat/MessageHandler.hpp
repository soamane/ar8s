#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include <string_view>

struct MessageHandler {
public:
    static bool IsRussianPhoneNumber(std::string_view message);
};

#endif // !MESSAGE_HANDLER_HPP