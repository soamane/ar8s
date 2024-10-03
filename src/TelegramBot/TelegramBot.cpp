#include "TelegramBot.hpp"

#include <regex>
#include <iostream>

TelegramBot::TelegramBot(std::string_view token) : m_bot(token.data()) {
    auto bot = this->m_bot.getApi().getMe();
    if (!bot) {
        throw std::runtime_error("Failed to initialize telegram-bot");
    }

    std::cout << "[+] Telegram bot successfully initialized:" << std::endl;
    std::cout << "\t[~] Name: " << bot->username << std::endl;
    std::cout << "\t[~] Id: " << bot->id << std::endl << std::endl;
}

TelegramBot::~TelegramBot() = default;

void TelegramBot::Start() {
    this->CreateEvents();
    this->CreateLoop();
}

void TelegramBot::CreateEvents() {
    this->OnCommandEvent("start", [&](TgBot::Message::Ptr message)
    {
        this->SendMessage(message->chat->id,
            "💬 Введи номер телефона в любом удобном формате. "
            "Бот автоматически сформирует его под нужный сервис.\n"
            "🇷🇺 Используйте только российский номер телефона."
        );
    });

    this->OnAnyMessageEvent([&](TgBot::Message::Ptr message)
    {
        if (message->text._Starts_with("/")) {
            return;
        }

        if (this->IsPhoneNumber(message->text)) {

        }
    });
}

void TelegramBot::CreateLoop() {
    try {
        TgBot::TgLongPoll longPoll(this->m_bot);
        while (true) {
            longPoll.start();
        }
    } catch (const TgBot::TgException& e) {
        std::cerr << "[!] TgLongPoll exception: " << e.what() << std::endl;
    }
}

TgBot::Message::Ptr TelegramBot::SendMessage(int64_t chatId, std::string_view text) {
    try {
        return this->m_bot.getApi().sendMessage(chatId, text.data());
    } catch (const TgBot::TgException& e) {
        std::cerr << "[!] Failed to send message: " << e.what() << std::endl;
    }
    return nullptr;
}

void TelegramBot::DeleteMessage(int64_t chatId, int32_t messageId) {
    try {
        this->m_bot.getApi().deleteMessage(chatId, messageId);
    } catch (const TgBot::TgException& e) {
        std::cerr << "[!] Failed to delete message: " << e.what() << std::endl;
    }
}

void TelegramBot::OnCommandEvent(std::string_view command, const std::function<void(TgBot::Message::Ptr)> function) {
    this->m_bot.getEvents().onCommand(command.data(), [function](TgBot::Message::Ptr message)
    {
        function(message);
    });
}

void TelegramBot::OnAnyMessageEvent(const std::function<void(TgBot::Message::Ptr)> function) {
    this->m_bot.getEvents().onAnyMessage([function](TgBot::Message::Ptr message)
    {
        function(message);
    });
}

bool TelegramBot::IsPhoneNumber(std::string_view phone) const {
    const std::regex pattern(R"(^\+7\d{10}$|^7\d{10}$|^8\d{10}$|^9\d{9}$)");
    return std::regex_match(phone.data(), pattern);
}