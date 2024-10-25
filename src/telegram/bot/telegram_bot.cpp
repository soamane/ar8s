#include "telegram_bot.hpp"

TelegramBot::TelegramBot(std::string_view token) : m_bot(token.data()), m_eventHandler(std::make_unique<EventHandler>(m_bot)) {
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
    this->m_eventHandler->CreateEvents();
    this->CreateLongPoll();
}

void TelegramBot::CreateLongPoll() {
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