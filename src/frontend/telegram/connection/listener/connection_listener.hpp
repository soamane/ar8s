#ifndef CONNECTION_LISTENER_HPP
#define CONNECTION_LISTENER_HPP

#include "../manager/connection_manager.hpp"
#include "../../event/handler/event_handler.hpp"

#include <tgbot/tgbot.h>

class ConnectionListener : public EventHandler {
public:
    ConnectionListener(TgBot::Bot& bot);
    void Listen();

private:
    TgBot::Bot& m_bot;

    ConnectionManager m_conManager;
};

#endif // !CONNECTION_LISTENER_HPP