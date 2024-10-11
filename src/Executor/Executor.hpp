#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include "../Parsers/Service/ServiceParser.hpp"
#include "../Parsers/Settings/SettingsParser.hpp"

#include <vector>
#include <optional>
#include <MyCurl.hpp>

using RESPONSE = std::pair<std::string, long>;

class Executor {
public:
    Executor(const Settings& settings, const std::vector<Service>& services);
    ~Executor();

    void Execute();

private:
    RESPONSE ExecuteRequest(std::shared_ptr<MyCurl> myCurl, const Service& service) const;
    void SetProxyToRequest(std::shared_ptr<MyCurl> myCurl);
    void SetUserAgentToRequest(Service& service);

    template<class T>
    std::optional<T> GetRandomObject(const std::vector<T>& vec);
private:
    Settings m_settings;
    std::vector<Service> m_services;
};

template<class T>
std::optional<T> Executor::GetRandomObject(const std::vector<T>& vec) {
    if (vec.empty()) {
        return std::nullopt;
    }

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<std::size_t> dis(0, vec.size() - 1);

    return vec[ dis(generator) ];
}

#endif // !EXECUTOR_HPP
