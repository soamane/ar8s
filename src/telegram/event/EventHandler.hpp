#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <tgbot/tgbot.h>

class EventHandler {
public:
    EventHandler(TgBot::Bot& bot);

    void CreateEvents();
    void CreateListenerLoop();

private:
    void OnAnyMessageEvent(std::function<void(TgBot::Message::Ptr)> function);
    void OnCommandEvent(std::string_view command, std::function<void(TgBot::Message::Ptr)> function);

private:
    TgBot::Message::Ptr SendMessage(int64_t chatId, std::string_view message);
    bool DeleteMessage(int64_t chatId, int32_t messageId);

private:
    TgBot::Bot& m_bot;
    bool m_phoneEntered;
};

#endif // !EVENT_HANDLER_HPP