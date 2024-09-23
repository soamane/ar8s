#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <memory>
#include <filesystem>

#include "../JsonParser/JsonParser.hpp"

struct Config
{
    bool useProxy;
    std::string proxyAddress;
    std::string proxyPassword;
    std::string targetPhone;
};

class ConfigParser : public JsonParser
{
public:
    ConfigParser();
    ~ConfigParser();

    const Config &GetParsedConfig();

private:
    void ParseConfig();

private:
    Config m_config;
};

#endif // !CONFIG_PARSER_HPP