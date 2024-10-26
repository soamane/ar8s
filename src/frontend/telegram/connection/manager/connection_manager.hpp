#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#include "../../user/data/user_data.hpp"

#include <optional>
#include <unordered_map>

class ConnectionManager {
public:
    void RegisterConnection(const UserData& userData);
    bool UnregisterConnection(const int64_t chatId);

    bool HasConnection(const int64_t chatId) const;
    std::optional<UserData> GetConnectionById(const int64_t chatId) const;

private:
    std::unordered_map<int64_t, UserData> m_connections;
};

#endif // !CONNECTION_MANAGER_HPP