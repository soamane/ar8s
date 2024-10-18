#include "config_parser.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

ConfigParser::ConfigParser(const std::filesystem::path& path) : m_path(path) {
    if (m_path.empty()) {
        throw std::invalid_argument("Configuration path is empty.");
    }
    if (!std::filesystem::exists(m_path)) {
        throw std::runtime_error("File " + m_path.string() + " does not exist.");
    }
}

nlohmann::json ConfigParser::Parse() const {
    std::ifstream file(m_path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + m_path.string());
    }

    nlohmann::json data;
    try {
        file >> data;
    } catch (const nlohmann::json::parse_error& e) {
        throw std::runtime_error("Failed to parse configuration file '" + m_path.string() + "': " + std::string(e.what()));
    }

    return data;
}
