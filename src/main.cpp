#include <memory>
#include <iostream>

#include "Parsers/Service/ServiceParser.hpp"
#include "Parsers/Settings/SettingsParser.hpp"
#include "Executor/Executor.hpp"

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
        SettingsParser settingsParser("configs\\settings.json");
        const Settings& settings = settingsParser.GetSettings();

        ServiceParser serviceParser(settings, "configs\\services.json");
        const std::vector<Service>& services = serviceParser.GetServices();

        Executor executor(settings, services);
        executor.Execute();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}