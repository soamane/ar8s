#include "ConfigParser.hpp"

#include <fstream>

ConfigParser::ConfigParser(const std::filesystem::path &path) : m_path(path)
{
}

ConfigParser::~ConfigParser()
{
}

nlohmann::json ConfigParser::Parse()
{
    if (this->m_path.empty()) {
        throw std::invalid_argument("Empty configuration path.");
    }

    if (!std::filesystem::exists(this->m_path)) {
        throw std::runtime_error("File " + this->m_path.string() + " doesn't exists.");
    }

    std::ifstream file(this->m_path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed open " + this->m_path.string() + " to parsing.");
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
