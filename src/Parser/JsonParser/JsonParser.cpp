#include "JsonParser.hpp"

JsonParser::JsonParser(const std::filesystem::path &path)
{
    this->OpenFile(path);
}

JsonParser::~JsonParser()
{
    this->CloseFile();
}

void JsonParser::OpenFile(const std::filesystem::path &path)
{
    if (!std::filesystem::exists(path))
    {
        throw std::runtime_error("File " + path.string() + " doesn't exists.");
    }

    this->m_file.open(path);
    if (!this->m_file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + path.string());
    }
}

void JsonParser::CloseFile()
{
    if (this->m_file.is_open())
    {
        this->m_file.close();
    }
}

const nlohmann::json JsonParser::Parse()
{
    nlohmann::json data;
    try
    {
        this->m_file >> data;
    }
    catch (const nlohmann::json::parse_error &e)
    {
        throw std::runtime_error("JSON parse error: " + std::string(e.what()));
    }

    return data;
}
