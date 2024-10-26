#ifndef CONNECTION_LISTENER_HPP
#define CONNECTION_LISTENER_HPP

#include "../manager/connection_manager.hpp"
#include "../../event/handler/event_handler.hpp"

#include <tgbot/tgbot.h>

class ConnectionListener {
public:
    ConnectionListener(TgBot::Bot& bot);
    void Listen();

private:
    void AcceptConnection(const int64_t currentChatId);
    void CreateEventHandler(std::shared_ptr<UserData> userData);
private:
    TgBot::Bot& m_bot;

    ConnectionManager m_connectionManager;
    std::vector<std::shared_ptr<EventHandler>> m_eventHandlers;
};

#endif // !CONNECTION_LISTENER_HPP