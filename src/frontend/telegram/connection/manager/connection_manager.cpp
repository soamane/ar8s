#include "connection_manager.hpp"

void ConnectionManager::RegisterConnection(std::shared_ptr<UserData> userData) {
    if (!HasConnection(userData->chatId)) {
        m_connections[userData->chatId] = userData;
    }
}

bool ConnectionManager::UnregisterConnection(int64_t chatId) {
    return m_connections.erase(chatId) > 0;
}

bool ConnectionManager::HasConnection(int64_t chatId) const {
    return m_connections.find(chatId) != m_connections.end();
}

std::shared_ptr<UserData> ConnectionManager::GetUserDataById(int64_t chatId) const {
    auto it = m_connections.find(chatId);
    return (it != m_connections.end()) ? it->second : nullptr;
}
