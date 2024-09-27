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
    std::cout << "[~] Parse placeholders: ";

    if (!data.contains("use-placeholders")) {
        throw std::runtime_error("Failed to parse 'use-placeholders', please check the correctness of the file.");
    }

    this->m_settings.usePlaceholders = data[ "use-placeholders" ];

    if (!data.contains("placeholders") || !data[ "placeholders" ].is_array()) {
        throw std::runtime_error("Failed to parse 'placeholders', please check the correctness of the file.");
    }

    const auto& placeholders = data[ "placeholders" ];

    for (const auto& object : placeholders) {
        Placeholder placeholder;
        placeholder.key = object[ "key" ];
        placeholder.value = object[ "value" ];

        this->m_settings.placeholders.push_back(placeholder);
    }

    std::cout << "Successful" << std::endl;
}

void SettingsParser::ParseAdditionals(const nlohmann::json& data) {
    if (!data.contains("attacks-count")) {
        throw std::runtime_error("Failed to parse 'use-placeholders', please check the correctness of the file.");
    }

    this->m_settings.attacksCount = data[ "attacks-count" ];

    if (!data.contains("loop-timeout")) {
        throw std::runtime_error("Failed to parse 'loop-timeout', please check the correctness of the file.");
    }

    this->m_settings.loopTimeout = data[ "loop-timeout" ];
}

void SettingsParser::ParseProxies(const nlohmann::json& data) {
    std::cout << "[~] Parse proxies: ";

    if (!data.contains("use-proxy")) {
        throw std::runtime_error("Failed to parse 'use-proxy', please check the correctness of the file.");
    }

    this->m_settings.useProxy = data[ "use-proxy" ];

    if (!data.contains("proxies") || !data[ "proxies" ].is_array()) {
        throw std::runtime_error("Failed to parse 'proxies', please check the correctness of the file.");
    }

    const auto& proxies = data[ "proxies" ];
    for (const auto& object : proxies) {
        Proxy proxy;
        proxy.address = object[ "address" ];
        proxy.username = object[ "username" ];
        proxy.password = object[ "password" ];

        this->m_settings.proxies.push_back(proxy);
    }

    std::cout << "Successful" << std::endl;
}
