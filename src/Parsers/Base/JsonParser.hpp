#ifndef JSON_PARSER_HPP
#define JSON_PARSER_HPP

#include <filesystem>
#include <nlohmann/json.hpp>

class JsonParser {
public:
    virtual ~JsonParser();
    virtual nlohmann::json Parse(const std::filesystem::path& path);

private:
};

#endif // !JSON_PARSER_HPP