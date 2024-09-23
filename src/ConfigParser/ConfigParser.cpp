#include "ConfigParser.hpp"

ConfigParser::ConfigParser() : JsonParser("config.json")
{
    this->ParseConfig();
}

ConfigParser::~ConfigParser()
{
}

const Config &ConfigParser::GetParsedConfig()
{
    return this->m_config;
}

void ConfigParser::ParseConfig()
{
    nlohmann::json data = this->Parse();

    this->m_config.useProxy = data["use-proxy"];
    this->m_config.proxyAddress = data["proxy"]["address"];
    this->m_config.proxyPassword = data["proxy"]["password"];
    this->m_config.targetPhone = data["target-phone"];
}
