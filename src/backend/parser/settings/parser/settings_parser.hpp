#ifndef SETTINGS_PARSER_HPP
#define SETTINGS_PARSER_HPP

#include "../settings.hpp"
#include "../../config/parser/config_parser.hpp"
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

class SettingsParser : public ConfigParser {
public:
    explicit SettingsParser(const std::filesystem::path& path);
    Settings& GetSettings();

private:
    void Load() override;
    void ParseAdditionals(const nlohmann::json& data);
    void ParseProxies(const nlohmann::json& data);
    void ParseUserAgents(const nlohmann::json& data);

    void CheckJsonKey(const nlohmann::json& data, const std::string& key) const;
    void CheckJsonArray(const nlohmann::json& data, const std::string& key) const;

    Settings m_settings;
};

#endif // !SETTINGS_PARSER_HPP
