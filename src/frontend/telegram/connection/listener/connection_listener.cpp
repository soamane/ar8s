#include "connection_listener.hpp"

#include "../../user/data/user_data.hpp"

ConnectionListener::ConnectionListener(TgBot::Bot& bot)
    : m_bot(bot) { }

void ConnectionListener::Listen() {
    this->m_bot.getEvents().onCommand("start", [this](TgBot::Message::Ptr message)
    {
        this->AcceptConnection(message->chat->id);
    });
}

void ConnectionListener::AcceptConnection(const int64_t currentChatId) {
    if (this->m_connectionManager.HasConnection(currentChatId)) {
        return;
    }

    std::shared_ptr<UserData> userData = std::make_shared<UserData>(currentChatId);
    this->m_connectionManager.RegisterConnection(userData);

    this->CreateEventHandler(userData);
}

void ConnectionListener::CreateEventHandler(std::shared_ptr<UserData> userData) {
    std::shared_ptr<EventHandler> eventHandler = std::make_shared<EventHandler>(this->m_bot, userData);
    eventHandler->Handle();

    this->m_eventHandlers.push_back(eventHandler);
}
