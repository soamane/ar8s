#include "EventHandler.hpp"
#include "../chat/MessageHandler.hpp"
#include "../../parsers/service/ServiceParser.hpp"
#include "../../parsers/settings/SettingsParser.hpp"
#include "../../executor/Executor.hpp"

#include <thread>
#include <iostream>

EventHandler::EventHandler(TgBot::Bot& bot) : m_bot(bot) { }

void EventHandler::CreateEvents() {
    OnCommandEvent("start", [this](TgBot::Message::Ptr message)
 {
     SendMessage(message->chat->id, "💬 Введи номер телефона в удобном формате.\n📲 🇷🇺 Укажи только российский номер.");
    });

    OnCommandEvent("execute", [this](TgBot::Message::Ptr message)
 {
     auto& user = m_users[ message->chat->id ];
     if (!user.phoneEntered || !user.attacksEntered) {
         SendErrorMessage(message->chat->id, message->messageId, "⚠️ Ты не можешь выполнить эту команду сейчас. Пожалуйста, убедитесь, что все данные введены корректно.");
         return;
     }
     LaunchAttack(message->chat->id, message);
    });

    OnAnyMessageEvent([this](TgBot::Message::Ptr message)
 {
     auto& user = m_users[ message->chat->id ];

     if (StringTools::startsWith(message->text, "/")) {
         return;
     }

     HandleUserMessage(message);
    });
}

void EventHandler::HandleUserMessage(TgBot::Message::Ptr message) {
    auto& user = m_users[ message->chat->id ];
    if (!user.phoneEntered) {
        ProcessPhoneNumber(message);
    } else if (!user.attacksEntered) {
        ProcessAttackCount(message);
    }
}

void EventHandler::LaunchAttack(int64_t chatId, TgBot::Message::Ptr message) {
    auto& user = m_users[ message->chat->id ];
    if (user.attackInProgress) {
        SendErrorMessage(chatId, message->chat->id, "⚠️ Атака уже выполняется.\nПопробуйте повторить команду после завершения. Вам придет сообщение сразу же после завершения.");
        return;
    }

    SendMessage(chatId, "🚀 Атака запущена на номер: +7" + user.phone + "\n⏳ Подождите завершения цикла.  Вам придет сообщение сразу же после окончания.");

    user.attackInProgress = true;

    std::thread([=]()
    {
        PerformExecutor(chatId, message);
        m_users.erase(message->chat->id);
    }).detach();
}

void EventHandler::ProcessPhoneNumber(TgBot::Message::Ptr message) {
    if (MessageHandler::IsRussianPhoneNumber(message->text)) {
        auto& user = m_users[ message->chat->id ];
        user.phone = MessageHandler::NormalizePhoneNumber(message->text);
        user.phoneEntered = !user.phone.empty();

        if (user.phoneEntered) {
            SendMessage(message->chat->id, "📲 Номер успешно обработан.\n💬 Введите количество минут для атаки (1-100):");
        } else {
            SendErrorMessage(message->chat->id, message->messageId, "❌ Некорректный номер телефона.");
        }
    } else {
        SendErrorMessage(message->chat->id, message->messageId, "❌ Некорректный формат номера телефона.");
    }
}

void EventHandler::ProcessAttackCount(TgBot::Message::Ptr message) {
    auto& user = m_users[ message->chat->id ];
    if (MessageHandler::IsDigitOnly(message->text)) {
        try {
            user.attacksCount = std::stoi(message->text);
            if (user.attacksCount < 1 || user.attacksCount > 100) {
                SendErrorMessage(message->chat->id, message->messageId, "❌ Количество минут должно быть от 1 до 100.");
                return;
            }
            user.attacksEntered = true;
            SendMessage(message->chat->id, "🔄 Количество минут для проведения атаки: " + std::to_string(user.attacksCount) + ". Введите команду /execute для запуска атаки.");
        } catch (const std::exception& e) {
            SendErrorMessage(message->chat->id, message->messageId, "❌ Некорректный ввод.");
        }
    } else {
        SendErrorMessage(message->chat->id, message->messageId, "❌ Введите количество минут цифрами.");
    }
}

void EventHandler::SendErrorMessage(int64_t chatId, int32_t messageId, const std::string& errorMessage) {
    auto answer = SendMessage(chatId, errorMessage);
    DeleteMessagesWithDelay(chatId, messageId, 0);
    DeleteMessagesWithDelay(chatId, answer->messageId, 2);
}

void EventHandler::DeleteMessagesWithDelay(int64_t chatId, int32_t messageId, int delay) {
    std::this_thread::sleep_for(std::chrono::seconds(delay));
    if (!DeleteMessage(chatId, messageId)) {
        std::cerr << "Warning: Could not delete message " << messageId << ". It might not exist or deletion is restricted." << std::endl;
    }
}

void EventHandler::PerformExecutor(int64_t chatId, TgBot::Message::Ptr message) {
    constexpr std::string_view settingsPath = "configs/settings.json";
    constexpr std::string_view servicesPath = "configs/services.json";

    SettingsParser settingsParser(settingsPath);
    Settings& settings = settingsParser.GetSettings();

    settings.phoneNumber = m_users[ message->chat->id ].phone;
    settings.attacksCount = m_users[ message->chat->id ].attacksCount;

    ServiceParser serviceParser(settings, servicesPath);
    const std::vector<Service>& services = serviceParser.GetServices();

    Executor executor(settings, services);
    executor.Execute();

    SendMessage(chatId, "✅ Атака завершена.\n\n📥 Использовано сервисов: " + std::to_string(services.size()) + "\nВремя выполнения (в минутах): " + std::to_string(this->m_users[ message->chat->id ].attacksCount));
    SendMessage(chatId, "💬 Чтобы снова воспользоваться ботом, введите команду /start");
}

void EventHandler::CreateListenerLoop() {
    TgBot::TgLongPoll longPoll(m_bot);
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
    m_bot.getEvents().onAnyMessage(std::move(function));
}

void EventHandler::OnCommandEvent(std::string_view command, std::function<void(TgBot::Message::Ptr)> function) {
    m_bot.getEvents().onCommand(command.data(), std::move(function));
}

TgBot::Message::Ptr EventHandler::SendMessage(int64_t chatId, std::string_view message) {
    return m_bot.getApi().sendMessage(chatId, message.data());
}

bool EventHandler::DeleteMessage(int64_t chatId, int32_t messageId) {
    try {
        return m_bot.getApi().deleteMessage(chatId, messageId);
    } catch (const TgBot::TgException& e) {
        std::cerr << "Error while deleting message: " << e.what() << std::endl;
        return false;
    }
}