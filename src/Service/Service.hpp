#ifndef SERVICE_HPP
#define SERVICE_HPP

#include <string>
#include <vector>

#include <MyCurl.hpp>

struct Service
{
    std::string url;
    std::string name;
    std::string payload;
    ProtocolType protocol;
    std::vector<std::string> headers;
};

#endif // !SERVICE_HPP