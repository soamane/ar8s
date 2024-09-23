cmake_minimum_required(VERSION 3.5.0)
project(ar8s LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(${PROJECT_NAME} 
    src/main/main.cpp
    src/Parser/JsonParser/JsonParser.cpp
    src/Parser/ConfigParser/ConfigParser.cpp
    src/Parser/ServiceParser/ServiceParser.cpp
    src/ServiceExecutor/ServiceExecutor.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/MyCurl/include  
    ${CMAKE_SOURCE_DIR}/src/Parser/JsonParser/ 
    ${CMAKE_SOURCE_DIR}/src/Parser/ConfigParser/ 
    ${CMAKE_SOURCE_DIR}/src/Parser/ServiceParser/ 
    ${CMAKE_SOURCE_DIR}/src/ServiceExecutor/ServiceExecutor/ 
    ${CURL_INCLUDE_DIRS} 
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE ${CURL_LIBRARIES} 
    PRIVATE ${CMAKE_SOURCE_DIR}/lib/MyCurl/lib/MyCurl_x64.lib
    PRIVATE Ws2_32
    PRIVATE Wldap32
    PRIVATE Normaliz
    PRIVATE Crypt32
    PRIVATE nlohmann_json::nlohmann_json
)
