#include "SettingsParser.hpp"

#include <iostream>

SettingsParser::SettingsParser(const std::filesystem::path& path) : ConfigParser(path) {
    this->Load();
}

Settings& SettingsParser::GetSettings() {
    return this->m_settings;
}

void SettingsParser::Load() {
    nlohmann::json data = this->Parse();
    if (data.empty()) {
        throw std::runtime_error("Settings data is empty.");
    }

    this->ParseProxies(data);
    this->ParseUserAgents(data);
    this->ParseAdditionals(data);
}

void SettingsParser::ParseAdditionals(const nlohmann::json& data) {
    this->CheckJsonKey(data, "attacks-count");
    this->m_settings.attacksCount = data.at("attacks-count");
}

void SettingsParser::ParseProxies(const nlohmann::json& data) {
    this->CheckJsonKey(data, "use-proxy");
    this->m_settings.useProxy = data.at("use-proxy");

    this->CheckJsonArray(data, "proxies");
    for (const auto& proxyObj : data[ "proxies" ]) {
        Proxy proxy;
        {
            proxy.address = proxyObj.at("address").get<std::string>();
            proxy.username = proxyObj.value("username", "");
            proxy.password = proxyObj.value("password", "");
        }

        this->m_settings.proxies.push_back(std::move(proxy));
    }
}

void SettingsParser::ParseUserAgents(const nlohmann::json& data) {
    this->CheckJsonKey(data, "use-useragent");
    this->m_settings.useUserAgent = data.at("use-useragent").get<bool>();

    this->CheckJsonArray(data, "user-agents");
    for (const auto& userAgentString : data[ "user-agents" ]) {
        UserAgent userAgent;
        {
            userAgent.name = userAgentString.get<std::string>();
        }

        this->m_settings.userAgents.push_back(userAgent);
    }
}


void SettingsParser::CheckJsonKey(const nlohmann::json& data, const std::string& key) const {
    if (!data.contains(key)) {
        throw std::runtime_error("Failed to parse '" + key + "', please check the correctness of the file.");
    }
}

void SettingsParser::CheckJsonArray(const nlohmann::json& data, const std::string& key) const {
    if (!data.contains(key) || !data[ key ].is_array()) {
        throw std::runtime_error("Failed to parse '" + key + "', please check the correctness of the file.");
    }
}
