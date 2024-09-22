#include "ConfigParser.hpp"

ConfigParser::ConfigParser() : JsonParser("settings.json")
{
}

ConfigParser::~ConfigParser()
{
}

const Config ConfigParser::ParseConfig()
{
    Config config;
    nlohmann::json data = this->Parse();

    config.useProxy = data["use-proxy"];
    config.proxyAddress = data["proxy"]["address"];
    config.proxyPassword = data["proxy"]["password"];
    config.targetPhone = data["target-phone"];

    return config;
}
