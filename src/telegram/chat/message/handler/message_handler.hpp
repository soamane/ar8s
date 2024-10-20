#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include <tgbot/tgbot.h>

class MessageHandler {
public:
    MessageHandler() = delete;
    MessageHandler(TgBot::Bot& bot);

public:
    bool DeleteChatMessage(int64_t chatId, int32_t messageId);
    TgBot::Message::Ptr SendChatMessage(int64_t chatId, std::string_view message);
    void SendErrorChatMessage(int64_t chatId, int32_t messageId, std::string_view message);

private:
    TgBot::Bot& m_bot;
};

#endif // !MESSAGE_HANDLER_HPP