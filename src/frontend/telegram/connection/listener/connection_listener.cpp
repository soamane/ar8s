#include "connection_listener.hpp"
#include "../../user/data/user_data.hpp"

ConnectionListener::ConnectionListener(TgBot::Bot& bot)
    : m_bot(bot) { }

void ConnectionListener::Listen() {
    m_bot.getEvents().onCommand("start", [this](TgBot::Message::Ptr message)
 {
     AcceptConnection(message->chat->id);
    });
}

void ConnectionListener::AcceptConnection(int64_t currentChatId) {
    auto currentUserData = m_connectionManager.GetUserDataById(currentChatId);
    if (currentUserData) {
        CreateEventHandler(currentUserData);
    } else {
        auto userData = std::make_shared<UserData>(currentChatId);
        m_connectionManager.RegisterConnection(userData);
        CreateEventHandler(userData);
    }
}

void ConnectionListener::CreateEventHandler(std::shared_ptr<UserData> userData) {
    if (auto currentEventHandler = m_connectionManager.GetEventHandlerByUserData(userData)) {
        currentEventHandler->Handle();
    } else {
        auto eventHandler = std::make_shared<EventHandler>(m_bot, userData);
        eventHandler->Handle();
        m_connectionManager.RegisterEventHandler(eventHandler, userData);
    }
}
