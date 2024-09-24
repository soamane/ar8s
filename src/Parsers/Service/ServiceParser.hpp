#ifndef SERVICE_PARSER_HPP
#define SERVICE_PARSER_HPP

#include <vector>
#include <filesystem>

#include "../Config/ConfigParser.hpp"

struct Service {
    bool var;
};

class ServiceParser : public ConfigParser {
public:
    ServiceParser(const std::filesystem::path& path);

    const std::vector<Service>& GetServices();
private:
    void Load() override;

private:
    std::vector<Service> m_services;
};

#endif // !SERVICE_PARSER_HPP