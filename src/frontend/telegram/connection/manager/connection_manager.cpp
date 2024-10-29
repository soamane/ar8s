#include "connection_manager.hpp"

void ConnectionManager::RegisterConnection(std::shared_ptr<UserData> userData) {
    if (HasConnection(userData->chatId)) {
        return;
    }

    m_connections[userData->chatId] = userData;
}

bool ConnectionManager::UnregisterConnection(const int64_t chatId) {
    return m_connections.erase(chatId) > 0;
}

bool ConnectionManager::HasConnection(const int64_t chatId) const {
    return m_connections.find(chatId) != m_connections.end();
}

std::shared_ptr<UserData> ConnectionManager::GetUserDataById(const int64_t chatId) const {
    auto it = m_connections.find(chatId);
    if (it != m_connections.end()) {
        return it->second;
    }

    return nullptr;
}
