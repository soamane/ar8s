#include "ServiceExecutor.hpp"

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
