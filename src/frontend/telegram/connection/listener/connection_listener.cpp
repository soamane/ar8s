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
    auto currentUserData = CreateConnection(currentChatId);
    CreateEventHandler(currentUserData);
}

std::shared_ptr<UserData> ConnectionListener::CreateConnection(int64_t currentChatId) {
    auto currentUserData = m_connectionManager.GetUserDataById(currentChatId);

    if (!currentUserData) {
        currentUserData = std::make_shared<UserData>(currentChatId);
        m_connectionManager.RegisterConnection(currentUserData);
    }

    return currentUserData;
}

void ConnectionListener::CreateEventHandler(std::shared_ptr<UserData> userData) {
    auto eventHandler = m_connectionManager.GetEventHandlerByUserData(userData);

    if (!eventHandler) {
        eventHandler = std::make_shared<EventHandler>(m_bot, userData);
        m_connectionManager.RegisterEventHandler(eventHandler, userData);
    }

    eventHandler->Handle();
}
