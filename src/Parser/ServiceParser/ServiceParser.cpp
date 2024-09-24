#include "ServiceParser.hpp"

#include <iostream>

ServiceParser::ServiceParser() : JsonParser("services.json") { }

ServiceParser::~ServiceParser() { }

const std::vector<Service>& ServiceParser::GetParsedServices() {
    if (this->services.empty()) {
        throw std::runtime_error("Service's array is empty.");
    }

    return this->services;
}

void ServiceParser::ParseServices() {
    std::cout << "[#] Parsing services.json file" << std::endl;

    nlohmann::json data = this->Parse();
    if (data.empty()) {
        throw std::runtime_error("Service's data is empty.");
    }

    if (!data.contains("services") || !data["services"].is_array()) {
        throw std::invalid_argument("Key 'services' is missing or not an array.");
    }

    for (const auto& obj : data["services"]) {
        Service service;

        service.name = obj.value("name", "");
        service.url = obj.value("url", "");
        service.payload = obj.value("payload", "");
        service.headers = obj.value("headers", std::vector<std::string>{});
        service.protocolType = static_cast<ProtocolType>(obj.value("protocolType", ProtocolType::HTTPS));
        service.requestType = static_cast<RequestType>(obj.value("requestType", RequestType::GET));

        this->services.push_back(service);
    }
}
