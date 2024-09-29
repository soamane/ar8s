#include "SettingsParser.hpp"

#include <iostream>

SettingsParser::SettingsParser(const std::filesystem::path& path) : ConfigParser(path) {
    this->Load();
}

const Settings& SettingsParser::GetSettings() {
    return this->m_settings;
}

void SettingsParser::Load() {
    std::cout << "[~] Loading settings config: ";

    nlohmann::json data = this->Parse();
    if (data.empty()) {
        throw std::runtime_error("Settings data is empty.");
    }

    this->ParseAdditionals(data);
    this->ParsePlaceholders(data);
    this->ParseProxies(data);
}

void SettingsParser::ParsePlaceholders(const nlohmann::json& data) {
    std::cout << "\t[~] Parse placeholders: ";

    const std::string placeholdersKey = "placeholders";
    const std::string usePlaceholdersKey = "use-placeholders";

    CheckJsonKey(data, usePlaceholdersKey);
    this->m_settings.usePlaceholders = data[ usePlaceholdersKey ];

    CheckJsonArray(data, placeholdersKey);

    for (const auto& object : data[ placeholdersKey ]) {
        Placeholder placeholder{
            object.at("key").get<std::string>(),
            object.at("value").get<std::string>()
        };
        this->m_settings.placeholders.push_back(std::move(placeholder));
    }

    std::cout << "Successful" << std::endl;
}

void SettingsParser::ParseAdditionals(const nlohmann::json& data) {
    CheckJsonKey(data, "attacks-count");
    this->m_settings.attacksCount = data.at("attacks-count");

    CheckJsonKey(data, "loop-timeout");
    this->m_settings.loopTimeout = data.at("loop-timeout");
}

void SettingsParser::ParseProxies(const nlohmann::json& data) {
    std::cout << "\t[~] Parse proxies: ";

    CheckJsonKey(data, "use-proxy");
    this->m_settings.useProxy = data.at("use-proxy");

    CheckJsonArray(data, "proxies");

    for (const auto& object : data[ "proxies" ]) {
        Proxy proxy{
            object.at("address").get<std::string>(),
            object.value("username", ""),
            object.value("password", "")
        };
        this->m_settings.proxies.push_back(std::move(proxy));
    }

    std::cout << "Successful" << std::endl;
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
