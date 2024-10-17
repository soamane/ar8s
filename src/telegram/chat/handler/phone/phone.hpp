#ifndef PHONE_HANDLER_HPP
#define PHONE_HANDLER_HPP

#include <string>
#include <string_view>

struct PhoneHandler {
    static const std::string FormatPhoneNumber(std::string_view phone);
};

#endif // !PHONE_HANDLER_HPP