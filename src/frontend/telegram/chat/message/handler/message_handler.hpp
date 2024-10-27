#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include "../../../user/data/user_data.hpp"

#include <memory>
#include <tgbot/tgbot.h>

class MessageHandler {
public:
    MessageHandler(TgBot::Bot& bot, std::shared_ptr<UserData> userData);

public:
    bool DeleteMessage(int32_t messageId);
    TgBot::Message::Ptr SendChatMessage(std::string_view message);

private:
    std::shared_ptr<UserData> m_user;
    TgBot::Bot& m_bot;
};

#endif // !MESSAGE_HANDLER_HPP