#include "connection_manager.hpp"

void ConnectionManager::RegisterConnection(const UserData& userData) {
    this->m_connections[userData.chatId] = userData;
}

bool ConnectionManager::UnregisterConnection(const int64_t chatId) {
    return this->m_connections.erase(chatId) > 0;
}

bool ConnectionManager::HasConnection(const int64_t chatId) const {
    return this->m_connections.find(chatId) != this->m_connections.end();
}

std::optional<UserData> ConnectionManager::GetConnectionById(const int64_t chatId) const {
    auto it = this->m_connections.find(chatId);
    if (it != this->m_connections.end()) {
        return it->second;
    }

    return std::nullopt;
}
