#include "connection_manager.hpp"

void ConnectionManager::RegisterConnection(std::shared_ptr<UserData> userData) {
    m_connections[userData->chatId] = userData;
}

bool ConnectionManager::UnregisterConnection(int64_t chatId) {
    if (!HasConnection(chatId)) {
        return false;
    }

    auto userData = m_connections[chatId];
    m_eventHandlers.erase(userData);
    return (m_connections.erase(chatId) > 0);
}

void ConnectionManager::RegisterEventHandler(std::shared_ptr<EventHandler> eventHandler, std::shared_ptr<UserData> userData) {
    m_eventHandlers[userData] = eventHandler;
}

bool ConnectionManager::EventHandlerRegisteredOnUser(std::shared_ptr<UserData> userData) const {
    return m_eventHandlers.find(userData) != m_eventHandlers.end();
}

bool ConnectionManager::HasConnection(int64_t chatId) const {
    return m_connections.count(chatId) > 0 && m_eventHandlers.count(m_connections.at(chatId)) > 0;
}

std::shared_ptr<UserData> ConnectionManager::GetUserDataById(int64_t chatId) const {
    auto it = m_connections.find(chatId);
    return (it != m_connections.end()) ? it->second : nullptr;
}

std::shared_ptr<EventHandler> ConnectionManager::GetEventHandlerByUserData(std::shared_ptr<UserData> userData) {
    auto it = m_eventHandlers.find(userData);
    return (it != m_eventHandlers.end()) ? it->second : nullptr;
}