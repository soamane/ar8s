#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#include "../../user/data/user_data.hpp"

#include <memory>
#include <unordered_map>

class ConnectionManager {
public:
    void RegisterConnection(std::shared_ptr<UserData> userData);
    bool UnregisterConnection(const int64_t chatId);

    bool HasConnection(const int64_t chatId) const;
    std::shared_ptr<UserData> GetUserDataById(const int64_t chatId) const;

private:
    std::unordered_map<int64_t, std::shared_ptr<UserData>> m_connections;
};

#endif // !CONNECTION_MANAGER_HPP