#ifndef JSON_PARSER_HPP
#define JSON_PARSER_HPP

#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

class JsonParser {
protected:
    JsonParser(const std::filesystem::path& path);
    ~JsonParser();

private:
    void OpenFile(const std::filesystem::path& path);
    void CloseFile();

protected:
    const nlohmann::json Parse();

private:
    std::fstream m_file;
};

#endif // !JSON_PARSER_HPP