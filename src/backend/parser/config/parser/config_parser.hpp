#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <filesystem>
#include <nlohmann/json.hpp>

class ConfigParser {
public:
    explicit ConfigParser(const std::filesystem::path& path);
    virtual ~ConfigParser() = default;

protected:
    virtual void Load() = 0;
    virtual nlohmann::json Parse() const;

private:
    std::filesystem::path m_path;
};

#endif // CONFIG_PARSER_HPP
