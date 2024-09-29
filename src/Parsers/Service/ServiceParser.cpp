#include "ServiceParser.hpp"
#include <iostream>

ServiceParser::ServiceParser(const Settings& settings, const std::filesystem::path& path)
    : m_settings(settings), ConfigParser(path) {
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

    if (!data.contains("services") || !data[ "services" ].is_array() || data[ "services" ].empty()) {
        throw std::runtime_error("Failed to parse 'services', please check the correctness of the file.");
    }

    for (const auto& object : data[ "services" ]) {
        Service service;

        service.name = object.at("name").get<std::string>();
        service.url = object.at("url").get<std::string>();
        service.payload = object.at("payload").get<std::string>();
        service.headers = object.at("headers").get<std::vector<std::string>>();

        if (this->m_settings.usePlaceholders) {
            this->ReplacePlaceholders(service.url);
            this->ReplacePlaceholders(service.payload);

            for (auto& header : service.headers) {
                this->ReplacePlaceholders(header);
            }
        }

        service.requestType = object.at("request-type").get<RequestType>();
        service.protocolType = object.at("protocol-type").get<ProtocolType>();

        this->m_services.push_back(std::move(service));
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
