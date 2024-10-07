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
    nlohmann::json data = this->Parse();
    if (data.empty()) {
        throw std::runtime_error("Service's data is empty.");
    }

    if (!data.contains("services") || !data[ "services" ].is_array() || data[ "services" ].empty()) {
        throw std::runtime_error("Failed to parse 'services', please check the correctness of the file.");
    }

    for (const auto& serviceObj : data[ "services" ]) {
        Service service;

        service.name = serviceObj.at("name").get<std::string>();
        service.url = serviceObj.at("url").get<std::string>();
        service.payload = serviceObj.at("payload").get<std::string>();
        service.headers = serviceObj.at("headers").get<std::vector<std::string>>();

        this->ReplacePhoneNumber(service.url);
        this->ReplacePhoneNumber(service.payload);

        for (auto& header : service.headers) {
            this->ReplacePhoneNumber(header);
        }

        service.requestType = serviceObj.at("request-type").get<RequestType>();
        service.protocolType = serviceObj.at("protocol-type").get<ProtocolType>();

        this->m_services.push_back(std::move(service));
    }
}

void ServiceParser::ReplacePhoneNumber(std::string& source) {
    std::size_t pos = 0;
    std::string_view placeholder = "${phone}";

    while ((pos = source.find("${phone}", pos)) != std::string::npos) {
        source.replace(pos, placeholder.length(), this->m_settings.phoneNumber);
        pos += this->m_settings.phoneNumber.length();
    }
}
