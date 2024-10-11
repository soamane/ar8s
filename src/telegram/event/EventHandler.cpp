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
        auto& user = this->m_users[ message->chat->id ];
        if (user.phoneEntered || user.iterationsEntered) {
            this->DeleteUserStatus(user);
            this->SendChatMessage(message->chat->id, "Видимо, вы уже ввели данные для проведения спам атаки.\n\nВы бы врядли попробовали ввести начальную команду снова, поэтому считаем, что вы что-то упустили. Мы стёрли предыдущие введенные данные чтобы вы смогли записать новые!");
        }
        this->SendChatMessage(message->chat->id, "💬 Введи номер телефона в удобном для тебя формате.\n\nБот использует только российские сервисы, поэтому принимает в работу исключительно российский номер телефона.");
    });

    OnCommandEvent("execute", [this](TgBot::Message::Ptr message)
    {
        const auto& user = this->m_users[ message->chat->id ];
        if (!user.phoneEntered || !user.iterationsEntered) {
            SendErrorMessage(message->chat->id, message->messageId, "⚠️ Ты не можешь выполнить эту команду сейчас. Пожалуйста, убедитесь, что все данные введены корректно.");
            return;
        }

        this->LaunchAttack(message->chat->id, message);
    });

    OnAnyMessageEvent([this](TgBot::Message::Ptr message)
    {
        if (StringTools::startsWith(message->text, "/")) {
            return;
        }

        this->HandleUserMessage(message);
    });
}

void EventHandler::HandleUserMessage(TgBot::Message::Ptr message) {
    const auto& user = this->m_users[ message->chat->id ];
    if (!user.phoneEntered) {
        this->ProcessPhoneNumber(message);
    } else if (!user.iterationsEntered) {
        this->ProcessAttackCount(message);
    }
}

void EventHandler::LaunchAttack(int64_t chatId, TgBot::Message::Ptr message) {
    auto& user = this->m_users[ message->chat->id ];
    if (user.attackInProgress) {
        this->SendErrorMessage(chatId, static_cast<int32_t>(message->chat->id), "⚠️ Атака уже выполняется.\nПопробуйте повторить команду после завершения. Вам придет сообщение сразу же после завершения");
        return;
    }

    this->SendChatMessage(chatId, "🚀 Атака запущена на номер: +7" + user.phone + "\n\n⏳ Подождите завершения цикла. Вам придет сообщение в этот чат сразу же после окончания");

    user.attackInProgress = true;
    std::thread([=]()
    {
        this->PerformExecutor(chatId, message);
        this->m_users.erase(message->chat->id);
    }).detach();
}

void EventHandler::ProcessPhoneNumber(TgBot::Message::Ptr message) {
    if (!MessageHandler::IsRussianPhoneNumber(message->text)) {
        this->SendErrorMessage(message->chat->id, message->messageId, "❌ Некорректный формат номера телефона.\n\nУбедитесь в правильности введеного номера и повторите попытку. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
        return;
    }

    auto& user = this->m_users[ message->chat->id ];
    user.phone = MessageHandler::NormalizePhoneNumber(message->text);
    user.phoneEntered = !user.phone.empty();

    if (user.phoneEntered) {
        this->SendChatMessage(message->chat->id, "📲 Номер телефона успешно считан.\n\n⏳ Введите желаемое время продолжительности спама. Учтите, что максимальное количество минут не должно превышать 100");
    } else {
        this->SendErrorMessage(message->chat->id, message->messageId, "❌ Некорректный номер телефона.\n\nУбедитесь в правильности введеного номера и повторите попытку. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
    }

}

void EventHandler::ProcessAttackCount(TgBot::Message::Ptr message) {
    auto& user = this->m_users[ message->chat->id ];
    if (!MessageHandler::IsDigitOnly(message->text)) {
        this->SendErrorMessage(message->chat->id, message->messageId, "❌ Некорректное значение времени.\n\nУбедитесь в правильности введеных данных. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
        return;
    }

    try {
        user.attackIterations = std::stoi(message->text);
        if (user.attackIterations < 1 || user.attackIterations > 100) {
            this->SendErrorMessage(message->chat->id, message->messageId, "❌ Количество минут должно быть от 1 до 100.\n\nУбедитесь в правильности введеных данных. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
            return;
        }

        user.iterationsEntered = true;
        this->SendChatMessage(message->chat->id, "🎲 Количество минут для проведения атаки: " + std::to_string(user.attackIterations) + "\n\nЧтобы начать спам по указанному номеру, введите команду /execute");
    } catch ([[maybe_unused]] const std::exception& e) {
        this->SendErrorMessage(message->chat->id, message->messageId, "❌ Некорректный ввод.\n\nУбедитесь в правильности введеных данных. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
    }
}

void EventHandler::SendErrorMessage(int64_t chatId, int32_t messageId, std::string_view errorMessage) {
    auto answer = this->SendChatMessage(chatId, errorMessage);
    if (answer == nullptr) {
        return;
    }

    this->DeleteMessagesWithDelay(chatId, messageId, 1);
    this->DeleteMessagesWithDelay(chatId, answer->messageId, 1);
}

void EventHandler::DeleteMessagesWithDelay(int64_t chatId, int32_t messageId, int delay) {
    std::this_thread::sleep_for(std::chrono::seconds(delay));
    if (!this->DeleteChatMessage(chatId, messageId)) {
        std::cerr << "Warning: Could not delete message " << messageId << ". It might not exist or deletion is restricted." << std::endl;
    }
}

void EventHandler::PerformExecutor(int64_t chatId, TgBot::Message::Ptr message) {
    const std::filesystem::path settingsPath = "configs\\settings.json";
    const std::filesystem::path servicesPath = "configs\\services.json";

    try {
        SettingsParser settingsParser(settingsPath);
        Settings& settings = settingsParser.GetSettings();
        {
            settings.phoneNumber = this->m_users[ message->chat->id ].phone;
            settings.attacksCount = this->m_users[ message->chat->id ].attackIterations;
        }

        ServiceParser serviceParser(settings, servicesPath);
        const std::vector<Service>& services = serviceParser.GetServices();

        std::unique_ptr<Executor> executor = std::make_unique<Executor>(settings, services);
        executor->Execute();

        this->SendChatMessage(chatId, "✅ Атака на указанный номер успешно завершена\n\nИспользовано спам-сервисов: " + std::to_string(services.size()) + "\n\nЧтобы снова воспользоваться функционалом, выполните команду /start");
    } catch (const std::exception& e) {
        std::cerr << "Execution exception: " << e.what() << std::endl;
        this->SendChatMessage(chatId, "❌ При выполнении атаки произошла непредвиденная ошибка.\n\nПожалуйста, повторите попытку позднее. В случае дальнейшего непредвиденного поведения обратитесь к разработчику: @soamane");
    }
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

TgBot::Message::Ptr EventHandler::SendChatMessage(int64_t chatId, std::string_view message) {
    try {
        return this->m_bot.getApi().sendMessage(chatId, message.data());
    } catch (const TgBot::TgException& e) {
        std::cerr << "Error while sending message: " << e.what() << std::endl;
        return nullptr;
    }
}

bool EventHandler::DeleteChatMessage(int64_t chatId, int32_t messageId) {
    try {
        return this->m_bot.getApi().deleteMessage(chatId, messageId);
    } catch (const TgBot::TgException& e) {
        std::cerr << "Error while deleting message: " << e.what() << std::endl;
        return false;
    }
}

void EventHandler::DeleteUserStatus(UserData& userData) {
    userData.phoneEntered = false;
    userData.iterationsEntered = false;
    userData.attackInProgress = false;
}