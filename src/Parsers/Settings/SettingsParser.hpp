#ifndef SETTINGS_PARSER_HPP
#define SETTINGS_PARSER_HPP

#include "../Config/ConfigParser.hpp"

#include <nlohmann/json.hpp>
#include <vector>
#include <string>

struct UserAgent {
    std::string name;
};

struct Proxy {
    std::string address;
    std::string username;
    std::string password;
};

struct Settings {
    int attacksCount;
    int loopTimeout;
    bool useProxy;
    bool useUserAgent;

    std::string phoneNumber;
    std::vector<Proxy> proxies;
    std::vector<UserAgent> userAgents;
};

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

private:
    Settings m_settings;
};

#endif // !SETTINGS_PARSER_HPP
