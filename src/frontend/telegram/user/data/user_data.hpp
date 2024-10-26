#ifndef USER_DATA_HPP
#define USER_DATA_HPP

#include <cstdint>

struct UserData {
    UserData();
    UserData(int64_t chatId);

    int64_t chatId;
};

#endif // !USER_DATA_HPP