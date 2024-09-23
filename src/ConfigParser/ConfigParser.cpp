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
    if (data.empty())
    {
        throw std::runtime_error("Config's data is empty.");
    }

    this->m_config.useProxy = data.value("use-proxy", false);
    this->m_config.proxyAddress = data.value("proxy", nlohmann::json{{"address", ""}}).value("address", "");
    this->m_config.proxyPassword = data.value("proxy", nlohmann::json{{"password", ""}}).value("password", "");
    this->m_config.targetPhone = data.value("phone", "");
}
