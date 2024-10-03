#ifndef TELEGRAM_BOT_HPP
#define TELEGRAM_BOT_HPP

#include <functional>
#include <string_view>
#include <tgbot/tgbot.h>

class TelegramBot {
public:
    TelegramBot(std::string_view token);
    ~TelegramBot();

    void Start();

private:
    void CreateEvents();
    void CreateLoop();

private:
    TgBot::Message::Ptr SendMessage(int64_t chatId, std::string_view text);
    void DeleteMessage(int64_t chatId, int32_t messageIds);

    void OnCommandEvent(std::string_view command, const std::function<void(TgBot::Message::Ptr)> function);
    void OnAnyMessageEvent(const std::function<void(TgBot::Message::Ptr)> function);

private:
    bool IsPhoneNumber(std::string_view phone) const;

private:
    TgBot::Bot m_bot;
};

#endif // !TELEGRAM_BOT_HPP