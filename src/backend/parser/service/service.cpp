#include "service.hpp"

#include "../../executor/helper/vector/vector_helper.hpp"

void Service::SetRandomUserAgent(const Settings& settings) {
    if (!settings.useUserAgent) {
        return;
    }

    std::string_view keyWord = "User-Agent: ";

    for (auto& header : headers) {
        if (header.find(keyWord) != 0) {
            continue;
        }

        auto optionalUserAgent = VectorHelper::GetRandomObject<UserAgent>(settings.userAgents);
        if (!optionalUserAgent.has_value()) {
            return;
        }

        const UserAgent& userAgent = optionalUserAgent.value();

        header = keyWord.data() + userAgent.name;

        break;
    }
}