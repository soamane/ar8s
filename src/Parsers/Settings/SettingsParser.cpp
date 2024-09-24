#include "SettingsParser.hpp"

SettingsParser::SettingsParser(const std::filesystem::path &path) : ConfigParser(path)
{
    this->Load();
}

const Settings &SettingsParser::GetSettings()
{
    return this->m_settings;
}

void SettingsParser::Load()
{
    nlohmann::json data = this->Parse();
    if (data.empty()) {
        throw std::runtime_error("Settings data is empty.");
    }

    m_settings.var = data["variable"];
}
