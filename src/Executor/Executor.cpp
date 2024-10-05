#include "Executor.hpp"
#include <thread>
#include <random>
#include <iostream>

Executor::Executor(const Settings& settings, const std::vector<Service>& services)
    : m_settings(settings), m_services(services) { }

Executor::~Executor() { }

void Executor::Execute() {
    ShowConfigs();
    std::cout << "[!] Executing services..." << std::endl;
    for (int i = 0; i < m_settings.attacksCount; ++i) {
        for (auto& service : m_services) {
            std::cout << "[Service]: " << service.name << std::endl;
            std::cout << "\t[URL]: " << service.url << std::endl;

            MyCurl myCurl;

            if (m_settings.useProxy) {
                auto proxyResult = this->GetRandomObject<Proxy>(m_settings.proxies);
                if (proxyResult) {
                    const Proxy& proxy = *proxyResult;
                    myCurl.UseProxyServer(proxy.address, proxy.username, proxy.password);
                    std::cout << "[+] Proxy used: " << proxy.address << std::endl;
                } else {
                    std::cout << "[-] Proxy not used: proxies' array is empty" << std::endl;
                }
            }

            if (this->m_settings.useUserAgent) {
                for (auto& header : service.headers) {
                    if (header.find("User-Agent:") == 0) {
                        auto userAgentResult = this->GetRandomObject<UserAgent>(this->m_settings.userAgents);
                        if (userAgentResult) {
                            const UserAgent& userAgent = *userAgentResult;
                            header = "User-Agent: " + userAgent.name;
                        }
                    }
                }
            }

            RESPONSE response = ExecuteRequest(myCurl, service);
            ProcessServiceResponse(response);
        }
        std::this_thread::sleep_for(std::chrono::seconds(m_settings.loopTimeout));
    }
}

RESPONSE Executor::ExecuteRequest(const MyCurl& myCurl, const Service& service) const {
    const curl_slist* headers = myCurl.AddHeaders(service.headers);
    if (!headers) {
        throw std::runtime_error("Failed to add headers for request: " + service.name);
    }

    switch (service.requestType) {
        case RequestType::GET:
            return myCurl.PerformGetRequest(service.protocolType, service.url, headers);

        case RequestType::POST:
            return myCurl.PerformPostRequest(service.protocolType, service.url, headers, service.payload);

        default:
            throw std::invalid_argument("Invalid request type for service '" + service.name + "'");
    }
}

void Executor::ProcessServiceResponse(const RESPONSE& response) const {
    std::cout << "\t[Status]: " << response.second << std::endl;
    if (response.second == 200) {
        std::cout << "\t[Response]: " << response.first << std::endl;
    }
    std::cout << std::endl;
}

void Executor::ShowConfigs() const {
    std::cout << "\n[!] Your config's settings:" << std::endl;
    std::cout << "\t[Settings]:" << std::boolalpha << std::endl;
    std::cout << "\t\t[Proxies]: " << m_settings.useProxy << ' ' << '[' << m_settings.proxies.size() << ']' << std::endl;
    std::cout << "\t\t[UserAgents]: " << m_settings.useUserAgent << ' ' << '[' << m_settings.userAgents.size() << ']' << std::endl;
    std::cout << "\t[Services]: " << m_services.size() << " object(-s)" << std::endl;
}
