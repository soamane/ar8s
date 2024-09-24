#include <memory>
#include <iostream>

#include "Parsers/Service/ServiceParser.hpp"
#include "Parsers/Settings/SettingsParser.hpp"
#include "Executor/Executor.hpp"

int main() {
    try {
        SettingsParser settingsParser("settings.json");
        const Settings& settings = settingsParser.GetSettings();

        ServiceParser serviceParser(settings, "services.json");
        const std::vector<Service>& services = serviceParser.GetServices();

        Executor executor(settings, services);
        executor.Execute();

    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}