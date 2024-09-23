#include "ConfigParser.hpp"

#include <iostream>

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
    std::cout << "[#] Parsing config.json file" << std::endl;
    nlohmann::json data = this->Parse();
    if (data.empty())
    {
        throw std::runtime_error("Config's data is empty.");
    }

    this->m_config.useProxy = data.value("use-proxy", false);
    this->m_config.proxyAddress = data.value("proxy", nlohmann::json{{"address", ""}}).value("address", "");
    this->m_config.proxyUsername = data.value("proxy", nlohmann::json{{"username", ""}}).value("username", "");
    this->m_config.proxyPassword = data.value("proxy", nlohmann::json{{"password", ""}}).value("password", "");
    this->m_config.phone = data.value("phone", "");
    this->m_config.attacks = data.value("attacks", 1);
}
