#ifndef SERVICE_HPP
#define SERVICE_HPP

#include "../settings/parser/settings_parser.hpp"
#include <vector>
#include <MyCurl.hpp>

struct Service {
    std::string name;
    std::string url;
    std::string payload;
    std::vector<std::string> headers;
    RequestType requestType;
    ProtocolType protocolType;

    void SetRandomUserAgent(const Settings& settings);
};

#endif // SERVICE_HPP
