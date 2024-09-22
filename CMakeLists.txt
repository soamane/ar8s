cmake_minimum_required(VERSION 3.5.0)
project(ar8s LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)

add_executable(${PROJECT_NAME} src/main/main.cpp)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/lib/MyCurl/include
)

set(CURL_DEFAULT_LIB Ws2_32 Crypt32 Wldap32 Normaliz)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE CURL::libcurl
    PRIVATE ${CMAKE_SOURCE_DIR}/lib/MyCurl/lib/MyCurl_x64.lib
    PRIVATE ${CURL_DEFAULT_LIB}
)

