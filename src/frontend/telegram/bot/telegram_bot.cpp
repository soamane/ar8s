#include "telegram_bot.hpp"

TelegramBot::TelegramBot(std::string_view token) : m_bot(token.data()), m_connectionListener(m_bot) { }

void TelegramBot::Run() {
    this->m_connectionListener.Listen();
    this->InitLongPoll();
}

void TelegramBot::InitLongPoll() {
    try {
        TgBot::TgLongPoll longPoll(this->m_bot);
        while (true) {
            longPoll.start();
        }

    } catch (const TgBot::TgException& e) {
        std::cerr << "TgLongPoll exception: " << e.what() << std::endl;
    }
}
