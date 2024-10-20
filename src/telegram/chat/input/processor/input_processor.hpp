#ifndef INPUT_PROCESSOR_HPP
#define INPUT_PROCESSOR_HPP

#include "../../../user/data/user_data.hpp"
#include "../../message/handler/message_handler.hpp"

#include <memory>
#include <tgbot/tgbot.h>

class InputProcessor {
public:
    InputProcessor() = delete;
    InputProcessor(std::shared_ptr<MessageHandler> messageHandler);

    void ProcessPhoneNumber(UserData& user, TgBot::Message::Ptr message);
    void ProcessAttackCount(UserData& user, TgBot::Message::Ptr message);

private:
    std::shared_ptr<MessageHandler> m_messageHandler;
};

#endif // !INPUT_PROCESSOR_HPP