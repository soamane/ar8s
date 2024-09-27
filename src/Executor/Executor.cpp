#include "Executor.hpp"

#include <thread>
#include <random>
#include <iostream>

Executor::Executor(const Settings& settings, const std::vector<Service>& services) : m_settings(settings), m_services(services) { }

Executor::~Executor() { }

void Executor::Execute() {
    this->ShowConfigs();
    std::cout << "[!] Execute services..." << std::endl;
    for (auto i = 0; i < this->m_settings.attacksCount; ++i) {
        for (const auto& service : this->m_services) {

            std::cout << "[Service]: " << service.name << std::endl;
            std::cout << "\t[URL]: " << service.url << std::endl;

            MyCurl myCurl;

            if (this->m_settings.useProxy) {
                const Proxy proxy = this->GetRandomProxy(this->m_settings.proxies);
                myCurl.UseProxyServer(proxy.address, proxy.username, proxy.password);
                std::cout << "[+] Proxy used: " << proxy.address << std::endl;
            }

            RESPONSE response = this->ExecuteRequest(myCurl, service);
            this->ProcessServiceResponse(response);
        }
        std::this_thread::sleep_for(std::chrono::seconds(this->m_settings.loopTimeout));
    }
}

const RESPONSE Executor::ExecuteRequest(const MyCurl& myCurl, const Service& service) {
    const curl_slist* headers = myCurl.AddHeaders(service.headers);
    if (headers == nullptr) {
        throw std::runtime_error("Failed to add headers for request.");
    }

    switch (service.requestType) {
        case RequestType::GET:
            return myCurl.PerformGetRequest(service.protocolType, service.url, headers);

        case RequestType::POST:
            return myCurl.PerformPostRequest(service.protocolType, service.url, headers, service.payload);

        default:
            throw std::invalid_argument("Invalid request type value (0 - GET / 1 - POST)");
            break;
    }
}

void Executor::ProcessServiceResponse(RESPONSE response) {
    std::cout << "\t[Status]: " << response.second << std::endl;
    std::cout << "\t[Response]: " << response.first << std::endl;
    std::cout << std::endl;
}

const Proxy Executor::GetRandomProxy(const std::vector<Proxy>& proxies) {
    if (proxies.empty()) {
        return Proxy();
    }

    if (proxies.size() == 1) {
        return proxies.front();
    }

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> dis(0, proxies.size() - 1);

    int index = dis(generator);

    return proxies.at(index);
}

void Executor::ShowConfigs() {
    std::cout << "\n[!] Your config's settings:" << std::endl;

    std::cout << "\t[Settings]:" << std::boolalpha << std::endl;
    std::cout << "\t\t[Proxies]: " << this->m_settings.useProxy << ' ' << '[' << this->m_settings.proxies.size() << ']' << std::endl;
    std::cout << "\t\t[Placeholders]: " << this->m_settings.usePlaceholders << ' ' << '[' << this->m_settings.placeholders.size() << ']' << std::endl;

    std::cout << "\t[Services]: " << this->m_services.size() << " object(-s)" << std::endl;
    std::cout << "\n[ar8s] Enter any key to execute services" << std::endl;
    std::cin.get();
}

