#ifndef SETTINGS_PARSER_HPP
#define SETTINGS_PARSER_HPP

#include "../Config/ConfigParser.hpp"

struct Proxy {
    std::string address;
    std::string username;
    std::string password;
};

struct Placeholder {
    std::string key;
    std::string value;
};

struct Settings {
    int attacksCount;
    int loopTimeout;
    bool useProxy;
    bool usePlaceholders;
    std::vector<Proxy> proxies;
    std::vector<Placeholder> placeholders;
};

class SettingsParser : public ConfigParser {
public:
    SettingsParser(const std::filesystem::path& path);
    const Settings& GetSettings();

private:
    void Load() override;

    void ParseAdditionals(const nlohmann::json& data);
    void ParsePlaceholders(const nlohmann::json& data);
    void ParseProxies(const nlohmann::json& data);

private:
    Settings m_settings;
};

#endif // !SETTINGS_PARSER_HPP