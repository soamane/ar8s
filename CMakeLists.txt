cmake_minimum_required(VERSION 3.5.0)
project(ar8s LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(Boost REQUIRED COMPONENTS asio property_tree)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(${PROJECT_NAME} 
    src/Parsers/Config/ConfigParser.cpp
    src/Parsers/Settings/SettingsParser.cpp
    src/Parsers/Service/ServiceParser.cpp
    src/Executor/Executor.cpp
    src/TelegramBot/TelegramBot.cpp
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/tgbot-cpp/include
    ${CMAKE_SOURCE_DIR}/lib/MyCurl/include/
    ${CURL_INCLUDE_DIRS} 
    ${Boost_INCLUDE_DIRS}  
    src/Parsers/Config/
    src/Parsers/Settings/
    src/Parsers/Service/
    src/Executor/
    src/TelegramBot/
)

# Линковка библиотек
target_link_libraries(${PROJECT_NAME} 
    PRIVATE ${CURL_LIBRARIES} 
    PRIVATE ${CMAKE_SOURCE_DIR}/lib/MyCurl/lib/MyCurl_x64.lib
    PRIVATE Ws2_32
    PRIVATE Wldap32
    PRIVATE Normaliz
    PRIVATE Crypt32
    PRIVATE nlohmann_json::nlohmann_json
    PRIVATE Boost::asio
    PRIVATE Boost::property_tree 
    PRIVATE OpenSSL::SSL
    PRIVATE OpenSSL::Crypto
    PRIVATE ${CMAKE_SOURCE_DIR}/lib/tgbot-cpp/lib/TgBot.lib
)
