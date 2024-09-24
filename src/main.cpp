#include <memory>
#include <iostream>

#include "Parsers/Config/ConfigParser.hpp"
#include "Parsers/Service/ServiceParser.hpp"
#include "Parsers/Settings/SettingsParser.hpp"

int main() {
    try {
        SettingsParser settingsParser("settings.json");
        const Settings& settings = settingsParser.GetSettings();

        std::cout << "settings.json >" << std::endl;
        std::cout << settings.var << std::endl;

        ServiceParser serviceParser("services.json");
        const std::vector<Service>& services = serviceParser.GetServices();

        std::cout << "services.json >" << std::endl;
        for (const auto& service : services) {
            std::cout << service.var << std::endl;
        }

    } catch(const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}