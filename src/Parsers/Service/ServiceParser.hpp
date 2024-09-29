#ifndef SERVICE_PARSER_HPP
#define SERVICE_PARSER_HPP

#include "../Config/ConfigParser.hpp"
#include "../Settings/SettingsParser.hpp"

#include <vector>
#include <filesystem>
#include <MyCurl.hpp>

struct Service {
    std::string name;
    std::string url;
    std::string payload;

    std::vector<std::string> headers;

    RequestType requestType;
    ProtocolType protocolType;
};

class ServiceParser : public ConfigParser {
public:
    ServiceParser(const Settings& settings, const std::filesystem::path& path);

    const std::vector<Service>& GetServices();

private:
    void Load() override;
    void ReplacePlaceholders(std::string& source);

private:
    std::vector<Service> m_services;
    Settings m_settings;
};

#endif // !SERVICE_PARSER_HPP