#include "event_handler.hpp"
#include "../../bot/executor/bot_executor.hpp"

EventHandler::EventHandler(TgBot::Bot& bot, std::shared_ptr<UserData> userData)
    : m_bot(bot), m_user(userData), m_messageHandler(std::make_shared<MessageHandler>(bot, userData)),
    m_inputProcessor(std::make_unique<InputProcessor>(userData)) { }

void EventHandler::Handle() {
    m_messageHandler->SendChatMessage("⚠️ Перед использованием Вы даете согласие, что не будете использовать функционал в незаконных целях.");
    m_messageHandler->SendChatMessage("📲 Введите номер телефона в любом удобном формате.\n\n*принимаются исключительно российские номера телефонов.");

    InitEvents();
}

void EventHandler::InitEvents() {
    RegisterClearCommand();
    RegisterStopCommand();
    RegisterHelpCommand();
    RegisterExecuteCommand();
    RegisterNonCommandMessageEvent();
}

void EventHandler::RegisterClearCommand() {
    onCommandEvent("clear", [this](TgBot::Message::Ptr message)
    {
        if (!ValidateUserData()) return;
        m_user->input->status->clear();
        m_messageHandler->SendChatMessage("♻ Введенные данные успешно стёрты. Укажите данные заново.");
    });
}

void EventHandler::RegisterStopCommand() {
    onCommandEvent("stop", [this](TgBot::Message::Ptr message)
    {
        if (!m_user->executor->attackInProgress) {
            m_messageHandler->SendChatMessage("❌ Эту команду нельзя выполнить сейчас.");
            return;
        }
        m_user->executor->attackStopped = true;
        m_user->input->status->clear();
        m_messageHandler->SendChatMessage("🚫 Запрос на остановку выполнен.");
    });
}

void EventHandler::RegisterHelpCommand() {
    onCommandEvent("help", [this](TgBot::Message::Ptr message)
    {
        m_messageHandler->SendChatMessage("Доступные команды бота");
        m_messageHandler->SendChatMessage("/help - вызов данного справочного сообщения\n/execute - запустить атаку (необходимо ввести необходимые данные перед использованием этой команды)\n/stop - принудительное завершение спам-атаки");
    });
}

void EventHandler::RegisterExecuteCommand() {
    onCommandEvent("execute", [this](TgBot::Message::Ptr message)
    {
        if (!ValidateUserData())
            return;

        if (m_user->executor->attackInProgress) {
            m_messageHandler->SendChatMessage("❌ Атака уже запущена");
            m_messageHandler->SendChatMessage("Используйте команду /stop для преждевременного завершения");
            return;
        }

        std::thread([this]
        {
            BotExecutor(m_user, m_messageHandler).Execute();
        }).detach();
    });
}

void EventHandler::RegisterNonCommandMessageEvent() {
    onNonCommandMessageEvent([this](TgBot::Message::Ptr message)
    {
        if (!m_user->input->status->phoneEntered) {
            if (!m_inputProcessor->ProcessPhoneNumber(message->text)) {
                m_messageHandler->SendChatMessage("❌ Некорректный номер телефона.\n\nУбедитесь в правильности введенного номера...");
                return;
            }
            m_messageHandler->SendChatMessage("📲 Номер телефона успешно считан.");
            m_messageHandler->SendChatMessage("⏳ Введите количество минут для атаки. Учтите, что максимальное количество не должно превышать 100");
        } else if (!m_user->input->status->attackTimeEntered) {
            if (!m_inputProcessor->ProcessAttackTime(message->text)) {
                m_messageHandler->SendChatMessage("❌ Некорректный ввод.");
                return;
            }
            m_messageHandler->SendChatMessage("🎲 Количество минут для атаки: " + std::to_string(m_user->input->attackTime));
            m_messageHandler->SendChatMessage("Чтобы начать спам по указанному номеру, введите команду /execute");
        }
    });
}

bool EventHandler::ValidateUserData() const {
    if (!m_user->isInitialized()) {
        m_messageHandler->SendChatMessage("❌ Эту команду нельзя выполнить сейчас.");
        return false;
    }
    return true;
}

void EventHandler::onCommandEvent(std::string_view command, std::function<void(TgBot::Message::Ptr message)> func) {
    m_bot.getEvents().onCommand(command.data(), std::move(func));
}

void EventHandler::onNonCommandMessageEvent(std::function<void(TgBot::Message::Ptr message)> func) {
    m_bot.getEvents().onNonCommandMessage(std::move(func));
}
