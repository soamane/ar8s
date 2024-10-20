#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include "../user/data/user_data.hpp"
#include "../chat/input/processor/input_processor.hpp"
#include "../chat/message/handler/message_handler.hpp"
#include "../bot/executor/bot_executor.hpp"

#include <string>
#include <functional>
#include <unordered_map>

#include <TgBot/TgBot.h>

class EventHandler {
public:
    explicit EventHandler(TgBot::Bot& bot);
    void CreateEvents();
    void CreateLongPoll();

private:
    void HandleUserMessage(TgBot::Message::Ptr message);

private:
    void LaunchAttackEvent(UserData& user);

private:
    void OnAnyMessageEvent(std::function<void(TgBot::Message::Ptr)> function);
    void OnCommandEvent(std::string_view command, std::function<void(TgBot::Message::Ptr)> function);

private:
    TgBot::Bot& m_bot;

    BotExecutor m_botExecutor;
    MessageHandler m_messageHandler;
    InputProcessor m_inputProcessor;

    std::unordered_map<int64_t, UserData> m_users;
};

#endif // EVENT_HANDLER_HPP