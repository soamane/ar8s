cmake_minimum_required(VERSION 3.5.0)

cmake_policy(SET CMP0167 NEW)

project(ar8s LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(Boost REQUIRED)

file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE HEADER_FILES
    ${CMAKE_SOURCE_DIR}/src/**/*.h
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CURL_INCLUDE_DIRS} 
    ${CMAKE_SOURCE_DIR}/vendor/lib/MyCurl/include/
    ${CMAKE_SOURCE_DIR}/vendor/lib/tgbot-cpp/include/
)

foreach(headerFile ${HEADER_FILES})
    get_filename_component(headerDir ${headerFile} DIRECTORY)
    list(APPEND INCLUDE_DIRS ${headerDir})
endforeach()

list(REMOVE_DUPLICATES INCLUDE_DIRS)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} 
    ${CURL_LIBRARIES} 
    ${OPENSSL_LIBRARIES} 
    ${Boost_LIBRARIES}
    ${CMAKE_SOURCE_DIR}/vendor/lib/MyCurl/lib/MyCurl_x64.lib
    ${CMAKE_SOURCE_DIR}/vendor/lib/tgbot-cpp/lib/TgBot.lib
    Ws2_32
    Wldap32
    Normaliz
    Crypt32
    nlohmann_json::nlohmann_json
    unofficial::sqlite3::sqlite3
)
