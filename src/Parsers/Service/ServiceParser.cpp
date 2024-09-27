#include "ServiceParser.hpp"

#include <iostream>

ServiceParser::ServiceParser(const Settings& settings, const std::filesystem::path& path) : m_settings(settings), ConfigParser(path) {
    this->Load();
}

const std::vector<Service>& ServiceParser::GetServices() {
    if (this->m_services.empty()) {
        throw std::runtime_error("Service's array is empty");
    }

    return this->m_services;
}

void ServiceParser::Load() {
    std::cout << "\n[~] Loading services config: ";

    nlohmann::json data = this->Parse();
    if (data.empty()) {
        throw std::runtime_error("Service's data is empty.");
    }

    const auto& services = data[ "services" ];
    if (!data.contains("services") || !services.is_array() || services.empty()) {
        throw std::runtime_error("Failed to parse 'services', please check the correctness of the file.");
    }

    for (const auto& object : services) {
        Service service;

        service.name = object[ "name" ];
        service.url = object[ "url" ];
        service.payload = object[ "payload" ];
        service.headers = object[ "headers" ];

        if (this->m_settings.usePlaceholders) {
            this->ReplacePlaceholders(service.url);
            this->ReplacePlaceholders(service.payload);

            for (auto& header : service.headers) {
                this->ReplacePlaceholders(header);
            }
        }

        service.requestType = object[ "request-type" ];
        service.protocolType = object[ "protocol-type" ];

        this->m_services.push_back(service);
    }
}

void ServiceParser::ReplacePlaceholders(std::string& source) {
    for (const auto& placeholder : this->m_settings.placeholders) {
        std::size_t pos = 0;
        while ((pos = source.find(placeholder.key, pos)) != std::string::npos) {
            source.replace(pos, placeholder.key.length(), placeholder.value);
            pos += placeholder.value.length();
        }
    }
}
