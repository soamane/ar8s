#include "Executor.hpp"

#include "helper/vector/vector_helper.hpp"
#include "handler/request/request_handler.hpp"

#include <random>
#include <thread>
#include <iostream>

Executor::Executor(const Settings& settings, const std::vector<Service>& services)
    : m_settings(settings), m_services(services) { }

Executor::~Executor() { }

void Executor::Execute(std::shared_ptr<UserData> user) {
    for (int i = 0; i < m_settings.attacksCount; ++i) {
        for (auto& service : m_services) {
            if (user->executor->attackStopped) {
                return;
            }

            service.SetRandomUserAgent(m_settings);

            std::async(std::launch::async, [this, &service]()
            {
                RequestHandler requestHandler;
                requestHandler.ExecuteRequest(m_settings, service);
            });
        }
    }
}