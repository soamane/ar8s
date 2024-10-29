#include "service_parser.hpp"

ServiceParser::ServiceParser(const Settings& settings, const std::filesystem::path& path) : m_settings(settings), ConfigParser(path) {
    Load();
}

const std::vector<Service>& ServiceParser::GetServices() {
    if (m_services.empty()) {
        throw std::runtime_error("Service's array is empty");
    }

    return m_services;
}

void ServiceParser::Load() {
    nlohmann::json data = Parse();
    if (data.empty()) {
        throw std::runtime_error("Service's data is empty.");
    }

    if (!data.contains("services") || !data["services"].is_array() || data["services"].empty()) {
        throw std::runtime_error("Failed to parse 'services', please check the correctness of the file.");
    }

    for (const auto& serviceObj : data["services"]) {
        Service service;

        service.name = serviceObj.at("name").get<std::string>();
        service.url = serviceObj.at("url").get<std::string>();
        service.payload = serviceObj.at("payload").get<std::string>();
        service.headers = serviceObj.at("headers").get<std::vector<std::string>>();

        ReplacePhoneNumber(service.url);
        ReplacePhoneNumber(service.payload);

        for (auto& header : service.headers) {
            ReplacePhoneNumber(header);
        }

        service.requestType = serviceObj.at("request-type").get<RequestType>();
        service.protocolType = serviceObj.at("protocol-type").get<ProtocolType>();

        m_services.push_back(std::move(service));
    }
}

void ServiceParser::ReplacePhoneNumber(std::string& source) {
    std::size_t pos = 0;
    std::string_view placeholder = "${phone}";

    while ((pos = source.find("${phone}", pos)) != std::string::npos) {
        source.replace(pos, placeholder.length(), m_settings.phoneNumber);
        pos += m_settings.phoneNumber.length();
    }
}
