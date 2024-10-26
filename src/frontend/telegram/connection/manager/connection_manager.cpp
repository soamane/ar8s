#include "connection_manager.hpp"

void ConnectionManager::RegisterConnection(std::shared_ptr<UserData> userData) {
    if (this->HasConnection(userData->chatId)) {
        return;
    }

    this->m_connections[userData->chatId] = userData;
}

bool ConnectionManager::UnregisterConnection(const int64_t chatId) {
    return this->m_connections.erase(chatId) > 0;
}

bool ConnectionManager::HasConnection(const int64_t chatId) const {
    return this->m_connections.find(chatId) != this->m_connections.end();
}

std::shared_ptr<UserData> ConnectionManager::GetUserDataById(const int64_t chatId) const {
    auto it = this->m_connections.find(chatId);
    if (it != this->m_connections.end()) {
        return it->second;
    }

    return nullptr;
}
