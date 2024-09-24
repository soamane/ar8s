#include <iostream>

#include "../ServiceExecutor/ServiceExecutor.hpp"

int main() {

    std::cout << R"(  
         _____    ____     __     ____       
        /\  _  \/\  _`\   /'_ `\ /\  _`\     
        \ \ \L\ \ \ \L\ \/\ \L\ \\ \,\L\_\   
         \ \  __ \ \ ,  /\/_> _ <_\/_\__ \   
          \ \ \/\ \ \ \\ \ /\ \L\ \ /\ \L\ \ 
           \ \_\ \_\ \_\ \_\ \____/ \ `\____\
            \/_/\/_/\/_/\/ /\/___/   \/_____/
    )" << std::endl;

    try {
        ServiceExecutor executor;
        executor.Execute();
    }
    catch (const std::exception& e) {
        std::cerr << "[Exception]: " << e.what() << std::endl;
    }

    return 0;
}