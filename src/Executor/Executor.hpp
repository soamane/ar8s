#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include "../Parsers/Service/ServiceParser.hpp"
#include "../Parsers/Settings/SettingsParser.hpp"

#include <vector>
#include <MyCurl.hpp>

using RESPONSE = std::pair<std::string, long>;

class Executor {
public:
    Executor(const Settings& settings, const std::vector<Service>& services);
    ~Executor();

    void Execute();

private:
    RESPONSE ExecuteRequest(const MyCurl& myCurl, const Service& service) const;
    void ProcessServiceResponse(const RESPONSE& response) const;
    Proxy GetRandomProxy(const std::vector<Proxy>& proxies) const;
    void ShowConfigs() const;

private:
    Settings m_settings;
    std::vector<Service> m_services;
};

#endif // EXECUTOR_HPP
