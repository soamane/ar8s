#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#include "../../user/data/user_data.hpp"
#include "../../event/handler/event_handler.hpp"

#include <memory>
#include <unordered_map>

class ConnectionManager {
public:
    void RegisterConnection(std::shared_ptr<UserData> userData);
    bool UnregisterConnection(int64_t chatId);

    void RegisterEventHandler(std::shared_ptr<EventHandler> eventHandler, std::shared_ptr<UserData> userData);

    bool EventHandlerRegisteredOnUser(std::shared_ptr<UserData> userData) const;

    bool HasConnection(int64_t chatId) const;

    std::shared_ptr<UserData> GetUserDataById(int64_t chatId) const;
    std::shared_ptr<EventHandler> GetEventHandlerByUserData(std::shared_ptr<UserData> userData);

private:
    std::unordered_map<int64_t, std::shared_ptr<UserData>> m_connections;
    std::unordered_map<std::shared_ptr<UserData>, std::shared_ptr<EventHandler>> m_eventHandlers;
};

#endif // CONNECTION_MANAGER_HPP
