#ifndef TELEGRAM_BOT_HPP
#define TELEGRAM_BOT_HPP

#include "../connection/listener/connection_listener.hpp"

class TelegramBot {
public:
    explicit TelegramBot(std::string_view token);

    void Run();

private:
    void InitLongPoll();

    TgBot::Bot m_bot;
    ConnectionListener m_connectionListener;
};

#endif // TELEGRAM_BOT_HPP
