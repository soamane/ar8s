#ifndef INPUT_PROCESSOR_HPP
#define INPUT_PROCESSOR_HPP

#include "../../../user/data/user_data.hpp"
#include "../../message/handler/message_handler.hpp"

#include <memory>

class InputProcessor {
public:
    InputProcessor() = delete;
    InputProcessor(std::shared_ptr<UserData> userData, std::shared_ptr<MessageHandler> messageHandler);

    void ProcessPhoneNumber(std::string_view message);
    void ProcessAttackTime(std::string_view message);

private:
    std::shared_ptr<UserData> m_user;
    std::shared_ptr<MessageHandler> m_messageHandler;
};

#endif // !INPUT_PROCESSOR_HPP