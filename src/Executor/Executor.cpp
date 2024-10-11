#include "Executor.hpp"

#include <random>
#include <thread>

Executor::Executor(const Settings& settings, const std::vector<Service>& services)
    : m_settings(settings), m_services(services) { }

Executor::~Executor() { }

void Executor::Execute() {
    for (int i = 0; i < m_settings.attacksCount; ++i) {
        for (auto& service : m_services) {
            std::shared_ptr<MyCurl> myCurl = std::make_unique<MyCurl>();

            this->SetProxyToRequest(myCurl);
            this->SetUserAgentToRequest(service);
            this->ExecuteRequest(myCurl, service);

        }
    }
}

RESPONSE Executor::ExecuteRequest(std::shared_ptr<MyCurl> myCurl, const Service& service) const {
    const curl_slist* headers = myCurl->AddHeaders(service.headers);
    if (!headers) {
        throw std::runtime_error("Failed to add headers for request: " + service.name);
    }

    switch (service.requestType) {
        case RequestType::GET:
            return myCurl->PerformGetRequest(service.protocolType, service.url, headers);

        case RequestType::POST:
            return myCurl->PerformPostRequest(service.protocolType, service.url, headers, service.payload);

        default:
            throw std::invalid_argument("Invalid request type for service '" + service.name + "'");
    }
}

void Executor::SetProxyToRequest(std::shared_ptr<MyCurl> myCurl) {
    if (!this->m_settings.useProxy) {
        return;
    }

    std::optional<Proxy> optionalProxy = this->GetRandomObject<Proxy>(this->m_settings.proxies);
    if (!optionalProxy.has_value()) {
        return;
    }

    const Proxy& proxy = optionalProxy.value();
    myCurl->UseProxyServer(proxy.address, proxy.username, proxy.password);
}

void Executor::SetUserAgentToRequest(Service& service) {
    if (!this->m_settings.useUserAgent) {
        return;
    }

    for (auto& header : service.headers) {
        if (header.find("User-Agent:") != 0) {
            continue;
        }

        auto optionalUserAgent = this->GetRandomObject<UserAgent>(this->m_settings.userAgents);
        if (!optionalUserAgent.has_value()) {
            return;
        }

        const UserAgent& userAgent = optionalUserAgent.value();
        header = "User-Agent: " + userAgent.name;

        break; // becouse 'user-agent' field replaced
    }
}
