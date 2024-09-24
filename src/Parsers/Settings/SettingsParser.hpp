#ifndef SETTINGS_PARSER_HPP
#define SETTINGS_PARSER_HPP

#include "../Config/ConfigParser.hpp"

struct Settings {
    bool var;
};

class SettingsParser : public ConfigParser {
public:
    SettingsParser(const std::filesystem::path& path);
    const Settings& GetSettings();
    
private:
    void Load() override;

private:
    Settings m_settings;
};

#endif // !SETTINGS_PARSER_HPP