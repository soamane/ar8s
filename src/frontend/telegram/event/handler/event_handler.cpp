#include "event_handler.hpp"

EventHandler::EventHandler(TgBot::Bot& bot, std::shared_ptr<UserData> userData)
    : m_bot(bot), m_user(userData), m_messageHandler(std::make_shared<MessageHandler>(bot, userData)),
    m_inputProcessor(std::make_unique<InputProcessor>(userData)) { }

void EventHandler::Handle() {
    this->m_messageHandler->SendChatMessage("⚠️ Перед использованием Вы даете согласие, что не будете использовать функционал в незаконных целях.");
    this->m_messageHandler->SendChatMessage("📲 Введите номер телефона в любом удобном формате.\n\n*принимаются исключительно российские номера телефонов.");

    this->InitEvents();
}

void EventHandler::InitEvents() {
    this->onCommandEvent("clear", [this](TgBot::Message::Ptr message)
    {
        if (!this->m_user->isInitialized()) {
            this->m_messageHandler->SendChatMessage("❌ Вы не можете использовать эту команду сейчас.\n\nУбедитесь в наличии введенных данных и повторите попытку.");
            return;
        }

        this->m_user->input->status->clear();
        this->m_messageHandler->SendChatMessage("♻ Ваши введенные данные успешно стёрты.\n\nПродолжайте работу с ботом, укажите нужные данные заново.");
    });
    this->onCommandEvent("execute", [this](TgBot::Message::Ptr message)
    {
        if (!this->m_user->isInitialized()) {
            this->m_messageHandler->SendChatMessage("❌ Эту команду нельзя выполнить сейчас.\n\nПеред использованием этой команды вы должны ввести необходимые данные.");
            return;
        }
    });

    this->onNonCommandMessageEvent([this](TgBot::Message::Ptr message)
    {
        if (!this->m_user->input->status->phoneEntered) {
            if (!this->m_inputProcessor->ProcessPhoneNumber(message->text)) {
                this->m_messageHandler->SendChatMessage("❌ Некорректный номер телефона.\n\nУбедитесь в правильности введеного номера и повторите попытку. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
                return;
            }

            this->m_messageHandler->SendChatMessage("📲 Номер телефона успешно считан.");
            this->m_messageHandler->SendChatMessage("⏳ Введите количество минут для атаки. Учтите, что максимальное количество не должно превышать 100");

        } else if (!this->m_user->input->status->attackTimeEntered) {
            if (!this->m_inputProcessor->ProcessAttackTime(message->text)) {
                this->m_messageHandler->SendChatMessage("❌ Некорректный ввод.\n\nУбедитесь в правильности введеных данных. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
                return;
            }

            this->m_messageHandler->SendChatMessage("🎲 Количество минут для проведения атаки: " + std::to_string(m_user->input->attackTime) + "\n\nЧтобы начать спам по указанному номеру, введите команду /execute");
        }
    });
}

void EventHandler::onCommandEvent(std::string_view command, std::function<void(TgBot::Message::Ptr message)> func) {
    this->m_bot.getEvents().onCommand(command.data(), std::move(func));
}

void EventHandler::onNonCommandMessageEvent(std::function<void(TgBot::Message::Ptr message)> func) {
    this->m_bot.getEvents().onNonCommandMessage(std::move(func));
}

