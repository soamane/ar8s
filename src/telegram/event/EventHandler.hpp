#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <TgBot/TgBot.h>
#include <string>
#include <functional>
#include <unordered_map>
#include <optional>

struct UserData {
    std::string phone;

    int attacksCount;
    bool phoneEntered;
    bool attacksEntered;
    bool attackInProgress;
};

class EventHandler {
public:
    explicit EventHandler(TgBot::Bot& bot);
    void CreateEvents();
    void CreateListenerLoop();

private:
    void HandleUserMessage(TgBot::Message::Ptr message);
    void LaunchAttack(int64_t chatId, TgBot::Message::Ptr message);
    void ProcessPhoneNumber(TgBot::Message::Ptr message);
    void ProcessAttackCount(TgBot::Message::Ptr message);
    void SendErrorMessage(int64_t chatId, int32_t messageId, const std::string& errorMessage);
    void DeleteMessagesWithDelay(int64_t chatId, int32_t messageId, int delay);
    void PerformExecutor(int64_t chatId, TgBot::Message::Ptr message);
    void OnAnyMessageEvent(std::function<void(TgBot::Message::Ptr)> function);
    void OnCommandEvent(std::string_view command, std::function<void(TgBot::Message::Ptr)> function);
    TgBot::Message::Ptr SendMessage(int64_t chatId, std::string_view message);
    bool DeleteMessage(int64_t chatId, int32_t messageId);

    TgBot::Bot& m_bot;
    std::unordered_map<int64_t, UserData> m_users;
};

#endif // EVENT_HANDLER_HPP