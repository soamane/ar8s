#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>
#include <vector>

struct UserAgent {
    std::string name;
};

struct Proxy {
    std::string address;
    std::string username;
    std::string password;
};

struct Settings {
    int attacksCount;
    bool useProxy;
    bool useUserAgent;

    std::string phoneNumber;
    std::vector<Proxy> proxies;
    std::vector<UserAgent> userAgents;
};

#endif // !SETTINGS_HPP