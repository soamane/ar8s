#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <string>
#include <functional>
#include <unordered_map>

#include <TgBot/TgBot.h>

struct UserData {
    std::string phone; // target phone
    int attackIterations;

    bool phoneEntered;      // phone's input flag
    bool iterationsEntered;   // iteration's input flag
    bool attackInProgress;  // attack status flag
};

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
    void SendErrorMessage(int64_t chatId, int32_t messageId, std::string_view errorMessage);
    void DeleteMessagesWithDelay(int64_t chatId, int32_t messageId, int delay);

private:
    void OnAnyMessageEvent(std::function<void(TgBot::Message::Ptr)> function);
    void OnCommandEvent(std::string_view command, std::function<void(TgBot::Message::Ptr)> function);

private:
    TgBot::Message::Ptr SendChatMessage(int64_t chatId, std::string_view message);
    bool DeleteChatMessage(int64_t chatId, int32_t messageId);

private:
    void DeleteUserStatus(UserData& userData);

private:
    TgBot::Bot& m_bot;
    std::unordered_map<int64_t, UserData> m_users;
};

#endif // EVENT_HANDLER_HPP