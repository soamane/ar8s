#include <memory>
#include <iostream>

#include "frontend/telegram/bot/telegram_bot.hpp"

int main() {
    std::cout << R"(
 ______  ____    ____     ____       
/\  _  \/\  _`\ /\  _`\  /\  _`\        Tool for sending service messages 
\ \ \L\ \ \ \L\ \ \ \L\ \\ \,\L\_\               to a phone number
 \ \  __ \ \ ,  /\ \  _ <'\/_\__ \   The author does not encourage use for harm.
  \ \ \/\ \ \ \\ \\ \ \L\ \ /\ \L\ \            
   \ \_\ \_\ \_\ \_\ \____/ \ `\____\       Created by github.com/soamane
    \/_/\/_/\/_/\/ /\/___/   \/_____/                                                                      
    )" << std::endl;

    try {
        TelegramBot bot("7437294115:AAE0CupyhrmmY9jzSZ3FbkiNf-P4AxcstHg");
        bot.Run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
