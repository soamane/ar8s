#ifndef USER_INPUT_HPP
#define USER_INPUT_HPP

#include <string>

struct UserInput {
    std::string phone;
    unsigned int attacksCount;

    void clear();
};

#endif // !USER_INPUT_HPP