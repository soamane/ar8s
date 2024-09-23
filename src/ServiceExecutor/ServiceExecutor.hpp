#ifndef SERVICE_EXECUTOR_HPP
#define SERVICE_EXECUTOR_HPP

#include <vector>
#include <MyCurl.hpp>

#include "../Parser/ConfigParser/ConfigParser.hpp"
#include "../Parser/ServiceParser/ServiceParser.hpp"

class ServiceExecutor
{
public:
    ServiceExecutor();
    ~ServiceExecutor();

    void Execute();

private:
    void Setup();

private:
    MyCurl m_myCurl;
    Config m_config;
    std::vector<Service> m_services;
};

#endif // !SERVICE_EXECUTOR_HPP
