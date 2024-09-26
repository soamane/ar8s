#include "Executor.hpp"

#include <iostream>

Executor::Executor(const Settings &settings, const std::vector<Service> &services) : m_settings(settings), m_services(services)
{
}

Executor::~Executor()
{
}

void Executor::Execute()
{
    std::cout << "[!] Execute services..." << std::endl;
    for (auto i = 0; i < this->m_settings.attacksCount; ++i) {
        for (const auto& service : this->m_services) {

            std::cout << "[Service]: " << service.name << std::endl;
            std::cout << "\t[URL]: " << service.url << std::endl;

            MyCurl myCurl;
            myCurl.UseProxyServer("46.38.128.6:5319", "user214609", "7owera");
            RESPONSE response = this->ExecuteRequest(myCurl, service);
            this->ProcessServiceResponse(response);
        }
    }
}

const RESPONSE Executor::ExecuteRequest(const MyCurl& myCurl, const Service &service)
{
    const curl_slist* headers = myCurl.AddHeaders(service.headers);
    if (headers == nullptr) {
        throw std::runtime_error("Failed to add headers for request.");
    }

    switch (service.requestType)
    {
    case RequestType::GET:
        return myCurl.PerformGetRequest(service.protocolType, service.url, headers);
    
    case RequestType::POST:
        return myCurl.PerformPostRequest(service.protocolType, service.url, headers, service.payload);
    
    default:
        throw std::invalid_argument("Invalid request type value (0 - GET / 1 - POST)");
        break;
    }
}

void Executor::ProcessServiceResponse(RESPONSE response)
{
    std::cout << "\t[Status]: " << response.second << std::endl;
    std::cout << "\t[Response]: " << response.first << std::endl;
}
