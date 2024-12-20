#ifndef CONNECTION_LISTENER_HPP
#define CONNECTION_LISTENER_HPP

#include "../manager/connection_manager.hpp"
#include "../../event/handler/event_handler.hpp"
#include "../../chat/message/handler/message_handler.hpp"

#include <tgbot/tgbot.h>
#include <vector>
#include <memory>

class ConnectionListener {
public:
    explicit ConnectionListener(TgBot::Bot& bot);
    void Listen();

private:
    void AcceptConnection(int64_t currentChatId);

    std::shared_ptr<UserData> CreateConnection(int64_t currentChatId);
    void CreateEventHandler(std::shared_ptr<UserData> userData);

private:
    TgBot::Bot& m_bot;
    ConnectionManager m_connectionManager;

    std::unordered_map<std::shared_ptr<UserData>, std::shared_ptr<EventHandler>> m_eventHandlers;
};

#endif // CONNECTION_LISTENER_HPP
