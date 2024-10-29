#include "message_handler.hpp"

#include <iostream>

MessageHandler::MessageHandler(TgBot::Bot& bot, std::shared_ptr<UserData> userData)
    : m_bot(bot), m_user(userData) { }

bool MessageHandler::DeleteMessage(int32_t messageId) {
    try {
        return m_bot.getApi().deleteMessage(m_user->chatId, messageId);
    } catch (const TgBot::TgException& e) {
        std::cerr << "An error occurred when deleting a message: " << e.what() << std::endl;
        return false;
    }
}

TgBot::Message::Ptr MessageHandler::SendChatMessage(std::string_view message) {
    try {
        return m_bot.getApi().sendMessage(m_user->chatId, message.data());
    } catch (const TgBot::TgException& e) {
        std::cerr << "There was an error sending a message: " << e.what() << std::endl;
        return nullptr;
    }
}
