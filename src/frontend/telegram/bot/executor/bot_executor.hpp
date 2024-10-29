#ifndef BOT_EXECUTOR_HPP
#define BOT_EXECUTOR_HPP

#include "../../user/data/user_data.hpp"
#include "../../chat/message/handler/message_handler.hpp"

#include <memory>
#include <filesystem>

class BotExecutor {
public:
    BotExecutor() = delete;
    BotExecutor(std::shared_ptr<UserData> user, std::shared_ptr<MessageHandler> messageHandler);

    void Execute();

private:
    std::shared_ptr<UserData> m_user;
    std::shared_ptr<MessageHandler> m_messageHandler;

    const std::filesystem::path m_settingsPath = "configs/settings.json";
    const std::filesystem::path m_servicesPath = "configs/services.json";
};

#endif // BOT_EXECUTOR_HPP
