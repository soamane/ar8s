#include "EventHandler.hpp"

#include "../chat/MessageHandler.hpp"

EventHandler::EventHandler(TgBot::Bot& bot) : m_bot(bot), m_phoneEntered(false) { }

void EventHandler::CreateEvents() {
    this->OnCommandEvent("start", [&](TgBot::Message::Ptr message)
    {
        this->SendMessage(message->chat->id, "💬 Введи номер телефона в любом удобном формате.\n🇷🇺 Используйте только российский номер телефона.");
    });

    this->OnCommandEvent("execute", [&](TgBot::Message::Ptr message)
    {
        if (!this->m_phoneEntered) {
            this->SendMessage(message->chat->id, "🗿 Перед использованием этой команды введите номер телефона");
            return;
        }

         // @TODO: Execute functional by current phone number
    });

    this->OnAnyMessageEvent([&](TgBot::Message::Ptr message)
    {
        if (MessageHandler::IsRussianPhoneNumber(message->text)) {
            this->m_phoneEntered = true;
            this->SendMessage(message->chat->id, "Номер телефона жертвы: " + message->text + "\nВведите команду /execute чтобы выполнить запросы к сервисам");
        }
    });
}

void EventHandler::CreateListenerLoop() {
    TgBot::TgLongPoll longPoll(this->m_bot);
    while (true) {
        try {
            longPoll.start();
        } catch (const TgBot::TgException& e) {
            std::cerr << "TgBot LongPoll exception: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    }
}

void EventHandler::OnAnyMessageEvent(std::function<void(TgBot::Message::Ptr)> function) {
    this->m_bot.getEvents().onAnyMessage(std::move(function));
}

void EventHandler::OnCommandEvent(std::string_view command, std::function<void(TgBot::Message::Ptr)> function) {
    this->m_bot.getEvents().onCommand(command.data(), std::move(function));
}

TgBot::Message::Ptr EventHandler::SendMessage(int64_t chatId, std::string_view message) {
    return this->m_bot.getApi().sendMessage(chatId, message.data());
}

bool EventHandler::DeleteMessage(int64_t chatId, int32_t messageId) {
    return this->m_bot.getApi().deleteMessage(chatId, messageId);
}
