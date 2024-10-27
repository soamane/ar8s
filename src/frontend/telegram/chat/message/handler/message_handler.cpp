#include "message_handler.hpp"

#include <iostream>

MessageHandler::MessageHandler(TgBot::Bot& bot, std::shared_ptr<UserData> userData)
: m_bot(bot), m_user(userData) { }

TgBot::Message::Ptr MessageHandler::SendChatMessage(std::string_view message)
{
    try {
        return this->m_bot.getApi().sendMessage(this->m_user->chatId, message.data());
    } catch (const TgBot::TgException& e) {
        std::cerr << "Fail send message: " << e.what() << std::endl;
    }
}
    