#include "service.hpp"
#include "../../executor/helper/vector/vector_helper.hpp"

void Service::SetRandomUserAgent(const Settings& settings) {
    if (!settings.useUserAgent) {
        return;
    }

    constexpr std::string_view keyWord = "User-Agent: ";

    for (auto& header : headers) {
        if (header.find(keyWord) == 0) {
            auto optionalUserAgent = VectorHelper::GetRandomObject<UserAgent>(settings.userAgents);
            if (optionalUserAgent) {
                header = keyWord.data() + optionalUserAgent->name;
            }
            break;
        }
    }
}
