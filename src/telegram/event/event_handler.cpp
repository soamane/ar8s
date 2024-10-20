#include "event_handler.hpp"

#include "../chat/input/handler/phone/phone_handler.hpp"
#include "../chat/input/validator/string/string_validator.hpp"

#include "../../parser/service/parser/service_parser.hpp"
#include "../../parser/settings/parser/settings_parser.hpp"

#include "../../executor/Executor.hpp"

#include <thread>
#include <iostream>

EventHandler::EventHandler(TgBot::Bot& bot) : m_bot(bot), m_messageHandler(std::make_unique<MessageHandler>(bot)) { }

void EventHandler::CreateEvents() {
    OnCommandEvent("start", [this](TgBot::Message::Ptr message)
    {
        auto& user = this->m_users[message->chat->id];
        if (user.inputStatus.phoneEntered || user.inputStatus.attacksEntered) {
            user.inputStatus.clear();
            this->m_messageHandler->SendChatMessage(message->chat->id, "Видимо, вы уже ввели данные для проведения спам атаки.\n\nВы бы врядли попробовали ввести начальную команду снова, поэтому считаем, что вы что-то упустили. Мы стёрли предыдущие введенные данные чтобы вы смогли записать новые!");
        }
        this->m_messageHandler->SendChatMessage(message->chat->id, "💬 Введи номер телефона в удобном для тебя формате.\n\nБот использует только российские сервисы, поэтому принимает в работу исключительно российский номер телефона.");
    });

    OnCommandEvent("execute", [this](TgBot::Message::Ptr message)
    {
        const auto& user = this->m_users[message->chat->id];
        if (!user.inputStatus.phoneEntered || !user.inputStatus.attacksEntered) {
            this->m_messageHandler->SendErrorMessage(message->chat->id, message->messageId, "⚠️ Ты не можешь выполнить эту команду сейчас. Пожалуйста, убедитесь, что все данные введены корректно.");
            return;
        }

        this->LaunchAttack(message->chat->id, message);
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
    const auto& user = this->m_users[message->chat->id];
    if (!user.inputStatus.phoneEntered) {
        this->ProcessPhoneNumber(message);
    } else if (!user.inputStatus.attacksEntered) {
        this->ProcessAttackCount(message);
    }
}

void EventHandler::LaunchAttack(int64_t chatId, TgBot::Message::Ptr message) {
    auto& user = this->m_users[message->chat->id];
    if (user.executorStatus.attackInProgress) {
        this->m_messageHandler->SendErrorMessage(chatId, static_cast<int32_t>(message->chat->id), "⚠️ Атака уже выполняется.\nПопробуйте повторить команду после завершения. Вам придет сообщение сразу же после завершения");
        return;
    }

    this->m_messageHandler->SendChatMessage(chatId, "🚀 Атака запущена на номер: +7" + user.input.phone + "\n\n⏳ Подождите завершения цикла. Вам придет сообщение в этот чат сразу же после окончания");

    user.executorStatus.attackInProgress = true;
    std::thread([=]()
    {
        this->PerformExecutor(chatId, message);
        this->m_users.erase(message->chat->id);
    }).detach();
}

void EventHandler::ProcessPhoneNumber(TgBot::Message::Ptr message) {
    if (!StringValidator::IsRussianPhoneNumber(message->text)) {
        this->m_messageHandler->SendErrorMessage(message->chat->id, message->messageId, "❌ Некорректный формат номера телефона.\n\nУбедитесь в правильности введеного номера и повторите попытку. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
        return;
    }

    auto& user = this->m_users[message->chat->id];
    user.input.phone = PhoneHandler::FormatPhoneNumber(message->text);
    if (user.input.phone.empty()) {
        throw std::runtime_error("Failed to format input phone number");
    }

    user.inputStatus.phoneEntered = !user.input.phone.empty();

    if (user.inputStatus.phoneEntered) {
        this->m_messageHandler->SendChatMessage(message->chat->id, "📲 Номер телефона успешно считан.\n\n⏳ Введите желаемое количество итераций. Учтите, что максимальное количество не должно превышать 100");
    } else {
        this->m_messageHandler->SendErrorMessage(message->chat->id, message->messageId, "❌ Некорректный номер телефона.\n\nУбедитесь в правильности введеного номера и повторите попытку. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
    }

}

void EventHandler::ProcessAttackCount(TgBot::Message::Ptr message) {
    auto& user = this->m_users[message->chat->id];
    if (!StringValidator::IsDigitOnly(message->text)) {
        this->m_messageHandler->SendErrorMessage(message->chat->id, message->messageId, "❌ Некорректное значение.\n\nУбедитесь в правильности введеных данных. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
        return;
    }

    try {
        user.input.attacksCount = std::stoi(message->text);
        if (user.input.attacksCount < 1 || user.input.attacksCount > 100) {
            this->m_messageHandler->SendErrorMessage(message->chat->id, message->messageId, "❌ Количество итераций должно быть от 1 до 100.\n\nУбедитесь в правильности введеных данных. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
            return;
        }

        user.inputStatus.attacksEntered = true;
        this->m_messageHandler->SendChatMessage(message->chat->id, "🎲 Количество итераций для проведения атаки: " + std::to_string(user.input.attacksCount) + "\n\nЧтобы начать спам по указанному номеру, введите команду /execute");
    } catch ([[maybe_unused]] const std::exception& e) {
        this->m_messageHandler->SendErrorMessage(message->chat->id, message->messageId, "❌ Некорректный ввод.\n\nУбедитесь в правильности введеных данных. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
    }
}

void EventHandler::PerformExecutor(int64_t chatId, TgBot::Message::Ptr message) {
    const std::filesystem::path settingsPath = "configs\\settings.json";
    const std::filesystem::path servicesPath = "configs\\services.json";

    try {
        SettingsParser settingsParser(settingsPath);
        Settings& settings = settingsParser.GetSettings();
        {
            auto& user = this->m_users[message->chat->id];
            settings.phoneNumber = user.input.phone;
            settings.attacksCount = user.input.attacksCount;
        }

        ServiceParser serviceParser(settings, servicesPath);
        const std::vector<Service>& services = serviceParser.GetServices();

        std::unique_ptr<Executor> executor = std::make_unique<Executor>(settings, services);
        executor->Execute();

        this->m_messageHandler->SendChatMessage(chatId, "✅ Атака на указанный номер успешно завершена\n\nИспользовано спам-сервисов: " + std::to_string(services.size()) + "\n\nЧтобы снова воспользоваться функционалом, выполните команду /start");
    } catch (const std::exception& e) {
        std::cerr << "Execution exception: " << e.what() << std::endl;
        this->m_messageHandler->SendChatMessage(chatId, "❌ При выполнении атаки произошла непредвиденная ошибка.\n\nПожалуйста, повторите попытку позднее. В случае дальнейшего непредвиденного поведения обратитесь к разработчику: @soamane");
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
