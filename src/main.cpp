#include "telegram/bot/telegram_bot.hpp"

#include <memory>
#include <iostream>

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
        std::string_view botToken = "7810469792:AAFPk333ZZMXEAFlOVK7iTBdLYAXH1_GYRE";
        auto bot = std::make_unique<TelegramBot>(botToken);
        bot->Start();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
