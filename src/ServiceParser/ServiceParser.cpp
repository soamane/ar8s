#include "ServiceParser.hpp"

#include <iostream>
#include <nlohmann/json.hpp>

ServiceParser::ServiceParser() : m_path("services.json")
{
}

ServiceParser::~ServiceParser()
{
    this->CloseFile();
}

const std::vector<Service> ServiceParser::ParseServices()
{
    this->OpenFile();

    nlohmann::json data;

    try {
        this->m_file >> data;
    } catch(const nlohmann::json::parse_error& exception) {
        throw std::runtime_error("Parse error in the document, please check the correctness of the file.");
    }
    
    std::vector<Service> services;
    for (const auto& service : data["services"]) {
        Service current;

        current.url = service["url"];
        current.name = service["name"];
        current.payload = service["payload"];
        current.protocol = service["protocol"];
        
        for (const auto& header : service["headers"]) {
            current.headers.push_back(header);
        }

        services.push_back(current);
    }

    this->CloseFile();
    return services;
}

void ServiceParser::OpenFile()
{
    if (!std::filesystem::exists(this->m_path)) {
        throw std::runtime_error("Service JSON file doesnt exists.");
    }

    this->m_file.open(this->m_path);
    if (!this->m_file.is_open()) {
        throw std::runtime_error("Failed to open service's settings.");
    }
}

void ServiceParser::CloseFile()
{
    if (this->m_file.is_open()) {
        this->m_file.close();
    }
}
