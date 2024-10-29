#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include "../parser/service/parser/service_parser.hpp"
#include "../parser/settings/parser/settings_parser.hpp"

#include "../../frontend/telegram/user/data/user_data.hpp"

class Executor {
public:
    Executor(const Settings& settings, const std::vector<Service>& services);
    ~Executor();

    void Execute(std::shared_ptr<UserData> user);
private:
    Settings m_settings;
    std::vector<Service> m_services;
};

#endif // !EXECUTOR_HPP
