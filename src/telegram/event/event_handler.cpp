#include "event_handler.hpp"
#include "../chat/input/handler/phone/phone_handler.hpp"
#include "../chat/input/validator/string/string_validator.hpp"

#include <thread>
#include <iostream>
#include <memory>

EventHandler::EventHandler(TgBot::Bot& bot)
    : m_bot(bot), m_messageHandler(bot),
    m_inputProcessor(m_messageHandler), m_botExecutor(m_messageHandler) { }

void EventHandler::CreateEvents() {
    OnCommandEvent("start", [this](TgBot::Message::Ptr message)
    {
        auto& user = m_users[message->chat->id];
        user.chatId = message->chat->id;

        if (user.inputStatus.phoneEntered || user.inputStatus.attacksEntered) {
            user.clear();
            m_messageHandler.SendChatMessage(user.chatId,
                "Видимо, вы уже ввели данные для проведения спам атаки.\n\n"
                "Мы стёрли предыдущие введенные данные, чтобы вы могли записать новые!");
        }

        m_messageHandler.SendChatMessage(user.chatId,
            "💬 Введи номер телефона в удобном для тебя формате.\n\n"
            "Бот использует только российские сервисы.");
    });

    OnCommandEvent("execute", [this](TgBot::Message::Ptr message)
    {
        auto& user = m_users[message->chat->id];
        if (!user.inputStatus.phoneEntered || !user.inputStatus.attacksEntered) {
            m_messageHandler.SendErrorChatMessage(user.chatId, message->messageId,
                "⚠️ Ты не можешь выполнить эту команду сейчас.");
            return;
        }

        LaunchAttackEvent(user);
    });

    OnCommandEvent("stop", [this](TgBot::Message::Ptr message)
    {
        auto& user = m_users[message->chat->id];
        if (user.executorStatus.attackInProgress) {
            user.executorStatus.attackStopped = true;
            m_messageHandler.SendChatMessage(user.chatId, "🛑 Запрос на остановку атаки принят.");
        } else {
            m_messageHandler.SendErrorChatMessage(user.chatId, message->messageId,
                "⚠️ Ты не можешь выполнить эту команду сейчас.");
        }
    });

    OnAnyMessageEvent([this](TgBot::Message::Ptr message)
    {
        if (!StringTools::startsWith(message->text, "/")) {
            HandleUserMessage(message);
        }
    });
}

void EventHandler::HandleUserMessage(TgBot::Message::Ptr message) {
    auto& user = m_users[message->chat->id];
    if (!user.inputStatus.phoneEntered) {
        m_inputProcessor.ProcessPhoneNumber(user, message);
    } else if (!user.inputStatus.attacksEntered) {
        m_inputProcessor.ProcessAttackCount(user, message);
    }
}

void EventHandler::LaunchAttackEvent(UserData& user) {
    if (user.executorStatus.attackInProgress) {
        m_messageHandler.SendChatMessage(user.chatId, "⚠️ Атака уже выполняется.");
        return;
    }

    m_messageHandler.SendChatMessage(user.chatId,
        "🚀 Атака запущена на номер: +7" + user.input.phone +
        "\n\n⏳ Подождите завершения цикла.");

    user.executorStatus.attackInProgress = true;
    std::thread([this, userChatId = user.chatId, &user]()
     {
         m_botExecutor.ExecuteForUser(user);
         user.executorStatus.attackInProgress = false;

         std::lock_guard<std::mutex> lock(this->m_mutex);
         m_users.erase(userChatId);
    }).detach();
}

void EventHandler::OnAnyMessageEvent(std::function<void(TgBot::Message::Ptr)> function) {
    m_bot.getEvents().onAnyMessage(std::move(function));
}

void EventHandler::OnCommandEvent(std::string_view command, std::function<void(TgBot::Message::Ptr)> function) {
    m_bot.getEvents().onCommand(command.data(), std::move(function));
}
