#ifndef TELEGRAM_BOT_HPP
#define TELEGRAM_BOT_HPP

#include "../event/event_handler.hpp"

#include <string_view>
#include <tgbot/tgbot.h>

class TelegramBot {
public:
    TelegramBot(std::string_view token);
    void Start();
    void CreateLongPoll();

private:
    TgBot::Bot m_bot;
    std::unique_ptr <EventHandler> m_eventHandler;
};

#endif // !TELEGRAM_BOT_HPP