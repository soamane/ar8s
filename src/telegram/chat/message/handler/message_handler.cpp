#include "message_handler.hpp"

MessageHandler::MessageHandler(TgBot::Bot& bot) : m_bot(bot) { }

bool MessageHandler::DeleteChatMessage(int64_t chatId, int32_t messageId) {
    try {
        return this->m_bot.getApi().deleteMessage(chatId, messageId);
    } catch (const TgBot::TgException& e) {
        std::cerr << "Error while deleting message: " << e.what() << std::endl;
        return false;
    }
}

TgBot::Message::Ptr MessageHandler::SendChatMessage(int64_t chatId, std::string_view message) {
    try {
        return this->m_bot.getApi().sendMessage(chatId, message.data());
    } catch (const TgBot::TgException& e) {
        std::cerr << "Error while sending message: " << e.what() << std::endl;
        return nullptr;
    }
}

void MessageHandler::SendErrorChatMessage(int64_t chatId, int32_t messageId, std::string_view message) {
    auto answer = this->SendChatMessage(chatId, message);
    if (answer == nullptr) {
        std::cerr << "Error while get chat message" << std::endl;
        return;
    }

    this->DeleteChatMessage(chatId, messageId);
    this->DeleteChatMessage(chatId, answer->messageId);
}
