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
    if (m_connectionManager.HasConnection(currentChatId)) {
        return;
    }

    auto userData = std::make_shared<UserData>(currentChatId);
    m_connectionManager.RegisterConnection(userData);
    CreateEventHandler(userData);
}

void ConnectionListener::CreateEventHandler(std::shared_ptr<UserData> userData) {
    auto eventHandler = std::make_shared<EventHandler>(m_bot, userData);
    eventHandler->Handle();
    m_eventHandlers.push_back(eventHandler);
}
