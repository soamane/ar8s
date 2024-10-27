#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include "../../user/data/user_data.hpp"

#include <memory>
#include <functional>
#include <tgbot/tgbot.h>

class EventHandler {
public:
    EventHandler(TgBot::Bot& bot, std::shared_ptr<UserData> userData);

    void Handle();

private:
    void onCommandEvent(std::string_view command, std::function<void(TgBot::Message::Ptr message)> func);
    void onNonCommandMessageEvent(std::function<void(TgBot::Message::Ptr message)> func);

private:
    TgBot::Bot& m_bot;
    std::shared_ptr<UserData> m_user;
};

#endif // !EVENT_HANDLER_HPP