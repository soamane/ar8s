#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include "../user/data/user_data.hpp"
#include "../chat/message/handler/message_handler.hpp"

#include <string>
#include <functional>
#include <unordered_map>

#include <TgBot/TgBot.h>

class EventHandler {
public:
    explicit EventHandler(TgBot::Bot& bot);
    void CreateEvents();
    void CreateListenerLoop();

private:
    void HandleUserMessage(TgBot::Message::Ptr message);

private:
    void LaunchAttack(int64_t chatId, TgBot::Message::Ptr message);
    void ProcessPhoneNumber(TgBot::Message::Ptr message);
    void ProcessAttackCount(TgBot::Message::Ptr message);
    void PerformExecutor(int64_t chatId, TgBot::Message::Ptr message);

private:
    void OnAnyMessageEvent(std::function<void(TgBot::Message::Ptr)> function);
    void OnCommandEvent(std::string_view command, std::function<void(TgBot::Message::Ptr)> function);

private:
    TgBot::Bot& m_bot;
    std::unique_ptr<MessageHandler> m_messageHandler;
    std::unordered_map<int64_t, UserData> m_users;
};

#endif // EVENT_HANDLER_HPP