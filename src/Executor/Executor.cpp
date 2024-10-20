#include "Executor.hpp"

#include "helper/vector/vector_helper.hpp"
#include "handler/request/request_handler.hpp"

#include <random>
#include <thread>
#include <iostream>

Executor::Executor(const Settings& settings, const std::vector<Service>& services)
    : m_settings(settings), m_services(services) { }

Executor::~Executor() { }

void Executor::Execute(const UserData& user) {
    for (int i = 0; i < m_settings.attacksCount; ++i) {
        if (user.executorStatus.attackStopped) {
            std::cerr << "Stop call" << std::endl;
            break;
        }

        for (auto& service : m_services) {
            service.SetRandomUserAgent(this->m_settings);

            RequestHandler requestHandler;
            requestHandler.ExecuteRequest(this->m_settings, service);
        }
    }
}