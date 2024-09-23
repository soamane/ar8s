#include <iostream>

#include "../Parser/ServiceParser/ServiceParser.hpp"

int main()
{
    try
    {
        ServiceParser parser;
        std::vector<Service> services = parser.GetParsedServices();

        for (const auto &service : services)
        {
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[Exception]: " << e.what() << std::endl;
    }

    return 0;
}