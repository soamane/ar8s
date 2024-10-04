#include "TelegramBot.hpp"

TelegramBot::TelegramBot(std::string_view token) : m_bot(token.data()) {
    try {
        auto me = this->m_bot.getApi().getMe();
        if (me == nullptr) {
            throw std::runtime_error("Failed initialize telegram bot");
        }

        std::cout << "Telegram bot successfully initialized: " << me->username << std::endl;
    } catch (const TgBot::TgException& e) {
        throw e;
    } catch (const std::exception& e) {
        throw e;
    }
}

void TelegramBot::Start() {
    this->CreateEvents();

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

void TelegramBot::CreateEvents() {
    this->OnCommandEvent("start", [&](TgBot::Message::Ptr message)
    {
        this->m_bot.getApi().sendMessage(message->chat->id, "Hello, pidoras!");
    });
}

void TelegramBot::OnCommandEvent(std::string_view command, std::function<void(TgBot::Message::Ptr)> function) {
    this->m_bot.getEvents().onCommand(command.data(), std::move(function));
}
