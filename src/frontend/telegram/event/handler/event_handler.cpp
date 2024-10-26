#include "event_handler.hpp"

EventHandler::EventHandler(TgBot::Bot& bot, std::shared_ptr<UserData> userData)
    : m_bot(bot), m_user(userData) { }

void EventHandler::Handle() {
    this->onAnyMessageEvent();
}


void EventHandler::onAnyMessageEvent() {
    this->m_bot.getEvents().onCommand("test", [this](TgBot::Message::Ptr message)
    {
        try {
            this->m_bot.getApi().sendMessage(message->chat->id, "Добро пожаловать в бота!" + std::to_string(this->m_user->chatId));
        } catch (const std::exception& e) {
            std::cerr << "Ошибка при отправке сообщения: " << e.what() << std::endl;
        }
    });
}
