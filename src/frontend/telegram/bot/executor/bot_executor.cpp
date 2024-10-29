#include "bot_executor.hpp"

#include "../../../../backend/parser/config/parser/config_parser.hpp"
#include "../../../../backend/parser/settings/parser/settings_parser.hpp"

#include "../../../../backend/executor/executor.hpp"

#include <iostream>

BotExecutor::BotExecutor(std::shared_ptr<UserData> user, std::shared_ptr<MessageHandler> messageHandler)
    : m_user(user), m_messageHandler(messageHandler) {

}

void BotExecutor::Execute() {
    this->m_user->executor->attackInProgress = true;

    try {
        SettingsParser settingsParser(this->m_settingsPath);
        Settings& settings = settingsParser.GetSettings();
        {
            settings.phoneNumber = this->m_user->input->phone;
            settings.attacksCount = this->m_user->input->attackTime;
        }

        ServiceParser serviceParser(settings, this->m_servicesPath);
        const std::vector<Service>& services = serviceParser.GetServices();

        std::unique_ptr<Executor> executor = std::make_unique<Executor>(settings, services);
        executor->Execute(this->m_user);

        this->m_messageHandler->SendChatMessage("✅ Атака на указанный номер успешно завершена");
        this->m_messageHandler->SendChatMessage("Чтобы снова воспользоваться функционалом, введите данные снова");

    } catch (const std::exception& e) {
        std::cerr << "Execution exception: " << e.what() << std::endl;
        this->m_messageHandler->SendChatMessage("❌ При выполнении атаки произошла непредвиденная ошибка.\n\nПожалуйста, повторите попытку позднее. В случае дальнейшего непредвиденного поведения обратитесь к разработчику: @soamane");
    }
}