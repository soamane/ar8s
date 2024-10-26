#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <tgbot/tgbot.h>

class EventHandler {
public:
    EventHandler(TgBot::Bot& bot);

protected:
    void OnRegisterEvent();

private:
    TgBot::Bot& m_bot;
};

#endif // !EVENT_HANDLER_HPP