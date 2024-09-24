#include "ServiceParser.hpp"

ServiceParser::ServiceParser(const std::filesystem::path &path) : ConfigParser(path)
{
    this->Load();
}

const std::vector<Service> &ServiceParser::GetServices()
{
    if (this->m_services.empty()) {
        throw std::runtime_error("Service's array is empty");
    }

    return this->m_services;
}

void ServiceParser::Load()
{
    nlohmann::json data = this->Parse();
    if (data.empty()) {
        throw std::runtime_error("Service's data is empty.");
    }

    const auto& services = data["services"];
    if (!data.contains("services") || !services.is_array() || services.empty()) {
        throw std::runtime_error("Failed to parse services, please check the correctness of the file.");
    }

    for (const auto& object : services) {
        Service service;   
        service.var = object["variable"];

        this->m_services.push_back(service);
    }
}
