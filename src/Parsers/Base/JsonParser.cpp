#include "JsonParser.hpp"

#include <fstream>

JsonParser::~JsonParser()
{
}

nlohmann::json JsonParser::Parse(const std::filesystem::path& path)
{
    if (path.empty()) {
        throw std::invalid_argument("Empty path for parsing file.");
    }

    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("File " + path.string() + " doesn't exists.");
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed open " + path.string() + " to parsing.");
    }

    nlohmann::json data;

    try {
        file >> data;
    } catch(const nlohmann::json::parse_error& e) {
        file.close();
        throw e;
    }

    return data;
}
