#include "event_handler.hpp"

EventHandler::EventHandler(TgBot::Bot& bot, std::shared_ptr<UserData> userData)
    : m_bot(bot), m_user(userData), m_messageHandler(std::make_shared<MessageHandler>(bot, userData)),
    m_inputProcessor(std::make_unique<InputProcessor>(userData, m_messageHandler)) { }

void EventHandler::Handle() {
    this->m_messageHandler->SendChatMessage("⚠️ Перед использованием вы даете согласие, что не будете использовать функционал в незаконных целях.");
    this->m_messageHandler->SendChatMessage("📲 Введи номер телефона в любом удобном для тебя формате.\n\n*принимаются исключительно российские номера телефонов");

    this->InitEvents();
}

void EventHandler::InitEvents() {
    this->onCommandEvent("execute", [this](TgBot::Message::Ptr message)
    {
        const auto& inputStatus = this->m_user->input->status;
        if (!inputStatus->attackTimeEntered || !inputStatus->phoneEntered) {

        } else {

        }
    });

    this->onNonCommandMessageEvent([this](TgBot::Message::Ptr message)
    {
        if (!this->m_user->input->status->phoneEntered) {
            this->m_inputProcessor->ProcessPhoneNumber(message->text);
        } else if (!this->m_user->input->status->attackTimeEntered) {
            this->m_inputProcessor->ProcessAttackTime(message->text);
        }
    });
}

void EventHandler::onCommandEvent(std::string_view command, std::function<void(TgBot::Message::Ptr message)> func) {
    this->m_bot.getEvents().onCommand(command.data(), std::move(func));
}

void EventHandler::onNonCommandMessageEvent(std::function<void(TgBot::Message::Ptr message)> func) {
    this->m_bot.getEvents().onNonCommandMessage(std::move(func));
}

