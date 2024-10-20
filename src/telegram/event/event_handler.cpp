#include "event_handler.hpp"

#include "../chat/input/handler/phone/phone_handler.hpp"
#include "../chat/input/validator/string/string_validator.hpp"

#include <thread>
#include <iostream>

EventHandler::EventHandler(TgBot::Bot& bot)
    : m_bot(bot), m_messageHandler(bot),
    m_inputProcessor(m_messageHandler),
    m_botExecutor(m_messageHandler) { }

void EventHandler::CreateEvents() {
    OnCommandEvent("start", [this](TgBot::Message::Ptr message)
    {
        auto& user = this->m_users[message->chat->id];
        user.chatId = message->chat->id;

        if (user.inputStatus.phoneEntered || user.inputStatus.attacksEntered) {
            user.clear();
            this->m_messageHandler.SendChatMessage(message->chat->id, "Видимо, вы уже ввели данные для проведения спам атаки.\n\nВы бы врядли попробовали ввести начальную команду снова, поэтому считаем, что вы что-то упустили. Мы стёрли предыдущие введенные данные чтобы вы смогли записать новые!");
        }
        this->m_messageHandler.SendChatMessage(message->chat->id, "💬 Введи номер телефона в удобном для тебя формате.\n\nБот использует только российские сервисы, поэтому принимает в работу исключительно российский номер телефона.");
    });

    OnCommandEvent("execute", [this](TgBot::Message::Ptr message)
    {
        auto& user = this->m_users[message->chat->id];
        if (!user.inputStatus.phoneEntered || !user.inputStatus.attacksEntered) {
            this->m_messageHandler.SendErrorMessage(message->chat->id, message->messageId, "⚠️ Ты не можешь выполнить эту команду сейчас. Пожалуйста, убедитесь, что все данные введены корректно.");
            return;
        }

        this->LaunchAttackEvent(user);
    });

    OnAnyMessageEvent([this](TgBot::Message::Ptr message)
    {
        if (StringTools::startsWith(message->text, "/")) {
            return;
        }

        try {
            this->HandleUserMessage(message);
        } catch (const std::exception& e) {
            std::cerr << "Failed to handle user message: " << e.what() << std::endl;
        }
    });
}

void EventHandler::HandleUserMessage(TgBot::Message::Ptr message) {
    auto& user = this->m_users[message->chat->id];
    if (!user.inputStatus.phoneEntered) {
        this->m_inputProcessor.ProcessPhoneNumber(user, message);
    } else if (!user.inputStatus.attacksEntered) {
        this->m_inputProcessor.ProcessAttackCount(user, message);
    }
}

void EventHandler::LaunchAttackEvent(UserData& user) {
    if (user.executorStatus.attackInProgress) {
        this->m_messageHandler.SendChatMessage(user.chatId, "⚠️ Атака уже выполняется.\nПопробуйте повторить команду после завершения. Вам придет сообщение сразу же после завершения");
        return;
    }

    this->m_messageHandler.SendChatMessage(user.chatId, "🚀 Атака запущена на номер: +7" + user.input.phone + "\n\n⏳ Подождите завершения цикла. Вам придет сообщение в этот чат сразу же после окончания");

    user.executorStatus.attackInProgress = true;
    std::thread([=]()
    {
        this->m_botExecutor.ExecuteForUser(user);
        this->m_users.erase(user.chatId);
    }).detach();
}


void EventHandler::CreateLongPoll() {
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
