#include "input_format.hpp"

#include <iterator>
#include <algorithm>

std::string InputFormat::FormatPhoneNumber(std::string_view phone) {
    std::string result;
    std::copy_if(phone.begin(), phone.end(), std::back_inserter(result), std::isdigit);

    if (result.size() == 11 && (result[0] == '7' || result[0] == '8')) {
        return result.substr(1);
    }

    if (result.size() == 10 && result[0] == '9') {
        return result;
    }

    return "";
}
