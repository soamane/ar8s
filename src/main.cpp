#include "Parsers/Service/ServiceParser.hpp"
#include "Parsers/Settings/SettingsParser.hpp"
#include "Executor/Executor.hpp"

#include <memory>
#include <iostream>

int main() {
    std::cout << R"(
 ______  ____    ____     ____       
/\  _  \/\  _`\ /\  _`\  /\  _`\        Tool for sending service messages 
\ \ \L\ \ \ \L\ \ \ \L\ \\ \,\L\_\               to a phone number
 \ \  __ \ \ ,  /\ \  _ <'\/_\__ \   The author does not encourage use for harm.
  \ \ \/\ \ \ \\ \\ \ \L\ \ /\ \L\ \            
   \ \_\ \_\ \_\ \_\ \____/ \ `\____\       Created by github.com/soamane
    \/_/\/_/\/_/\/ /\/___/   \/_____/                                                                      
    )" << std::endl;

    try {
        // Путь к конфигурационным файлам
        const std::string settingsPath = "configs\\settings.json";
        const std::string servicesPath = "configs\\services.json";

        SettingsParser settingsParser(settingsPath);
        const Settings& settings = settingsParser.GetSettings();

        ServiceParser serviceParser(settings, servicesPath);
        const std::vector<Service>& services = serviceParser.GetServices();

        Executor executor(settings, services);
        executor.Execute();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
