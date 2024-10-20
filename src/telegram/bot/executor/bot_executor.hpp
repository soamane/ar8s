#ifndef BOT_EXECUTOR_HPP
#define BOT_EXECUTOR_HPP

#include "../../user/data/user_data.hpp"
#include "../../chat/message/handler/message_handler.hpp"

#include <filesystem>

class BotExecutor {
public:
    BotExecutor() = delete;
    BotExecutor(MessageHandler& messageHandler);

    void ExecuteForUser(const UserData& user);

private:
    MessageHandler& m_messageHandler;
    const std::filesystem::path settingsPath = "configs\\settings.json";
    const std::filesystem::path servicesPath = "configs\\services.json";
};

#endif // !BOT_EXECUTOR_HPP