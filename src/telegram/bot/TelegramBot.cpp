#include "TelegramBot.hpp"

TelegramBot::TelegramBot(std::string_view token) : m_bot(token.data()), EventHandler(m_bot) {
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
    this->CreateListenerLoop();
}