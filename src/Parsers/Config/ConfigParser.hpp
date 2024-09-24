#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <filesystem>
#include <nlohmann/json.hpp>

class ConfigParser {
public:
    ConfigParser(const std::filesystem::path& path);

    virtual ~ConfigParser() = default;
    virtual nlohmann::json Parse();

private:
    std::filesystem::path m_path;
};

#endif // !CONFIG_PARSER_HPP