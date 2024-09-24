#ifndef JSON_PARSER_HPP
#define JSON_PARSER_HPP

#include <filesystem>
#include <nlohmann/json.hpp>

class JsonParser {
public:
    JsonParser(const std::filesystem::path& path);
    
    virtual ~JsonParser();
    virtual nlohmann::json Parse();

private:
    std::filesystem::path m_path;
};

#endif // !JSON_PARSER_HPP