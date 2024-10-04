#ifndef TELEGRAM_BOT_HPP
#define TELEGRAM_BOT_HPP

#include <string_view>
#include <tgbot/tgbot.h>

class TelegramBot {
public:
    TelegramBot(std::string_view token);
    void Start();

private:
    void CreateEvents();

private:
    void OnCommandEvent(std::string_view command, std::function<void(TgBot::Message::Ptr)> function);

private:
    TgBot::Bot m_bot;
};

#endif // !TELEGRAM_BOT_HPP