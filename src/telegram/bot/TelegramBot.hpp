#ifndef TELEGRAM_BOT_HPP
#define TELEGRAM_BOT_HPP

#include "../event/EventHandler.hpp"

#include <string_view>
#include <tgbot/tgbot.h>

class TelegramBot : public EventHandler {
public:
    TelegramBot(std::string_view token);
    void Start();

private:
    TgBot::Bot m_bot;
};

#endif // !TELEGRAM_BOT_HPP