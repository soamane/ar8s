#ifndef INPUT_PROCESSOR_HPP
#define INPUT_PROCESSOR_HPP

#include "../../../user/data/user_data.hpp"
#include "../../message/handler/message_handler.hpp"

#include <memory>

class InputProcessor {
public:
    InputProcessor() = delete;
    InputProcessor(std::shared_ptr<UserData> userData);

    bool ProcessPhoneNumber(std::string_view message);
    bool ProcessAttackTime(std::string_view message);

private:
    std::shared_ptr<UserData> m_user;
};

#endif // !INPUT_PROCESSOR_HPP