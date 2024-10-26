#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include "../../../parser/service/service.hpp"
#include "../../../parser/settings/settings.hpp"

#include <MyCurl.hpp>

using RESPONSE = std::pair<std::string, long>;

class RequestHandler {
public:
    RequestHandler();
    const RESPONSE ExecuteRequest(const Settings& settings, const Service& service) const;

private:
    void SetProxyToRequest(const Settings& settings) const;

private:
    std::unique_ptr<MyCurl> m_myCurl;
};

#endif // !REQUEST_HANDLER_HPP
