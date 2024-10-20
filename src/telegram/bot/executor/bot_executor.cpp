#include "bot_executor.hpp"

#include "../../../parser/config/parser/config_parser.hpp"
#include "../../../parser/settings/parser/settings_parser.hpp"
#include "../../../executor/executor.hpp"

BotExecutor::BotExecutor(MessageHandler& messageHandler) : m_messageHandler(messageHandler) { }

void BotExecutor::ExecuteForUser(const UserData& user) {
    try {
        SettingsParser settingsParser(this->settingsPath);
        Settings& settings = settingsParser.GetSettings();
        {
            settings.phoneNumber = user.input.phone;
            settings.attacksCount = user.input.attacksCount;
        }

        ServiceParser serviceParser(settings, this->servicesPath);
        const std::vector<Service>& services = serviceParser.GetServices();

        std::unique_ptr<Executor> executor = std::make_unique<Executor>(settings, services);
        executor->Execute();

        this->m_messageHandler.SendChatMessage(user.chatId, "✅ Атака на указанный номер успешно завершена\n\nИспользовано спам-сервисов: " + std::to_string(services.size()) + "\n\nЧтобы снова воспользоваться функционалом, выполните команду /start");
    } catch (const std::exception& e) {
        std::cerr << "Execution exception: " << e.what() << std::endl;
        this->m_messageHandler.SendChatMessage(user.chatId, "❌ При выполнении атаки произошла непредвиденная ошибка.\n\nПожалуйста, повторите попытку позднее. В случае дальнейшего непредвиденного поведения обратитесь к разработчику: @soamane");
    }
}
