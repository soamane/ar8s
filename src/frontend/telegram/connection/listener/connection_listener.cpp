#include "connection_listener.hpp"

#include "../../user/data/user_data.hpp"

ConnectionListener::ConnectionListener(TgBot::Bot& bot)
    : EventHandler(bot), m_bot(bot) { }

void ConnectionListener::Listen() {
    this->m_bot.getEvents().onCommand("start", [this](TgBot::Message::Ptr message)
    {

    });
}
