#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <vector>
#include <MyCurl.hpp>

#include "../Parsers/Service/ServiceParser.hpp"
#include "../Parsers/Settings/SettingsParser.hpp"

using RESPONSE = std::pair<std::string, long>;

class Executor {
public:
    Executor(const Settings& settings, const std::vector<Service>& services);
    ~Executor();

    void Execute();

private:
    const RESPONSE ExecuteRequest(const MyCurl& myCurl, const Service& service);
    void ProcessServiceResponse(RESPONSE response);

private:
    Settings m_settings;
    std::vector<Service> m_services;
};

#endif // !EXECUTOR_HPP