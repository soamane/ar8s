#include "event_handler.hpp"

EventHandler::EventHandler(TgBot::Bot& bot, std::shared_ptr<UserData> userData)
    : m_bot(bot), m_user(userData) { }

void EventHandler::Handle() {
    this->onCommandEvent("command", [this](TgBot::Message::Ptr message)
    {

    });

    this->onNonCommandMessageEvent([this](TgBot::Message::Ptr message)
    {

    });
}

void EventHandler::onCommandEvent(std::string_view command, std::function<void(TgBot::Message::Ptr message)> func) {
    this->m_bot.getEvents().onCommand(command.data(), std::move(func));
}

void EventHandler::onNonCommandMessageEvent(std::function<void(TgBot::Message::Ptr message)> func) {
    this->m_bot.getEvents().onNonCommandMessage(std::move(func));
}

