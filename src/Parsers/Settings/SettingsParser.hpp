#ifndef SETTINGS_PARSER_HPP
#define SETTINGS_PARSER_HPP

#include "../Config/ConfigParser.hpp"

struct Placeholder {
    std::string key;
    std::string value;
};

struct Settings {
    int attacksCount;
    bool usePlaceholders;
    std::vector<Placeholder> placeholders;
};

class SettingsParser : public ConfigParser {
public:
    SettingsParser(const std::filesystem::path& path);
    const Settings& GetSettings();

private:
    void Load() override;
    void ParsePlaceholders(const nlohmann::json& data);

private:
    Settings m_settings;
};

#endif // !SETTINGS_PARSER_HPP