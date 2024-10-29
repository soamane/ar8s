#include "request_handler.hpp"
#include "../../helper/vector/vector_helper.hpp"

#include <optional>
#include <stdexcept>

RequestHandler::RequestHandler() : m_myCurl(std::make_unique<MyCurl>()) { }

RESPONSE RequestHandler::ExecuteRequest(const Settings& settings, const Service& service) const {
    SetProxyToRequest(settings);

    const curl_slist* headers = m_myCurl->AddHeaders(service.headers);
    if (!headers) {
        throw std::runtime_error("Failed to add headers for request: " + service.name);
    }

    switch (service.requestType) {
        case RequestType::GET:
            return m_myCurl->PerformGetRequest(service.protocolType, service.url, headers);

        case RequestType::POST:
            return m_myCurl->PerformPostRequest(service.protocolType, service.url, headers, service.payload);

        default:
            throw std::invalid_argument("Invalid request type for service: " + service.name);
    }
}

void RequestHandler::SetProxyToRequest(const Settings& settings) const {
    if (!settings.useProxy) return;

    auto optionalProxy = VectorHelper::GetRandomObject<Proxy>(settings.proxies);
    if (!optionalProxy) return;

    const Proxy& proxy = *optionalProxy;
    m_myCurl->UseProxyServer(proxy.address, proxy.username, proxy.password);
}
