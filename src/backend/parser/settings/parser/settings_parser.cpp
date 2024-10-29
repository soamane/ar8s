#include "settings_parser.hpp"

SettingsParser::SettingsParser(const std::filesystem::path& path) : ConfigParser(path) {
    Load();
}

Settings& SettingsParser::GetSettings() {
    return m_settings;
}

void SettingsParser::Load() {
    nlohmann::json data = Parse();
    if (data.empty()) {
        throw std::runtime_error("Settings data is empty.");
    }

    ParseProxies(data);
    ParseUserAgents(data);
    ParseAdditionals(data);
}

void SettingsParser::ParseAdditionals(const nlohmann::json& data) {
    CheckJsonKey(data, "attacks-count");
    m_settings.attacksCount = data.at("attacks-count");
}

void SettingsParser::ParseProxies(const nlohmann::json& data) {
    CheckJsonKey(data, "use-proxy");
    m_settings.useProxy = data.at("use-proxy");

    CheckJsonArray(data, "proxies");
    for (const auto& proxyObj : data["proxies"]) {
        Proxy proxy;
        {
            proxy.address = proxyObj.at("address").get<std::string>();
            proxy.username = proxyObj.value("username", "");
            proxy.password = proxyObj.value("password", "");
        }

        m_settings.proxies.push_back(std::move(proxy));
    }
}

void SettingsParser::ParseUserAgents(const nlohmann::json& data) {
    CheckJsonKey(data, "use-useragent");
    m_settings.useUserAgent = data.at("use-useragent").get<bool>();

    CheckJsonArray(data, "user-agents");
    for (const auto& userAgentString : data["user-agents"]) {
        UserAgent userAgent;
        {
            userAgent.name = userAgentString.get<std::string>();
        }

        m_settings.userAgents.push_back(userAgent);
    }
}


void SettingsParser::CheckJsonKey(const nlohmann::json& data, const std::string& key) const {
    if (!data.contains(key)) {
        throw std::runtime_error("Failed to parse '" + key + "', please check the correctness of the file.");
    }
}

void SettingsParser::CheckJsonArray(const nlohmann::json& data, const std::string& key) const {
    if (!data.contains(key) || !data[key].is_array()) {
        throw std::runtime_error("Failed to parse '" + key + "', please check the correctness of the file.");
    }
}
