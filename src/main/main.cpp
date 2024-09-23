#include <iostream>
#include "../ConfigParser/ConfigParser.hpp"

int main()
{
    try
    {
        ConfigParser config;
        Config cfg = config.GetParsedConfig();

        std::cout << "use-proxy: " << cfg.useProxy << std::endl;
        std::cout << "\taddress: " << cfg.proxyAddress << std::endl;
        std::cout << "\tproxyPassword: " << cfg.proxyPassword << std::endl;
        std::cout << "phone: " << cfg.targetPhone << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[Exception]: " << e.what() << std::endl;
    }

    return 0;
}