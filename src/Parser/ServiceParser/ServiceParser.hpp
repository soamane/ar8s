#ifndef SERVICE_PARSER_HPP
#define SERVICE_PARSER_HPP

#include <vector>
#include <MyCurl.hpp>

#include "../JsonParser/JsonParser.hpp"

struct Service {
    std::string name;
    std::string url;

    std::string payload;
    std::vector<std::string> headers;

    ProtocolType protocolType;
    RequestType requestType;
};

class ServiceParser : JsonParser {
public:
    ServiceParser();
    ~ServiceParser();

    const std::vector<Service>& GetParsedServices();

private:
    void ParseServices();

private:
    std::vector<Service> services;
};

#endif // !SERVICE_PARSER_HPP