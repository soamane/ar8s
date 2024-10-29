#ifndef SERVICE_PARSER_HPP
#define SERVICE_PARSER_HPP

#include "../service.hpp"
#include "../../config/parser/config_parser.hpp"
#include "../../settings/parser/settings_parser.hpp"

#include <vector>
#include <filesystem>

class ServiceParser : public ConfigParser {
public:
    ServiceParser(const Settings& settings, const std::filesystem::path& path);

    const std::vector<Service>& GetServices();

private:
    void Load() override;
    void ReplacePhoneNumber(std::string& source);

private:
    Settings m_settings;
    std::vector<Service> m_services;
};

#endif // SERVICE_PARSER_HPP
