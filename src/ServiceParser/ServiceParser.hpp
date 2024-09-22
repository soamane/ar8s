#ifndef SERVICE_PARSER_HPP
#define SERVICE_PARSER_HPP

#include <fstream>
#include <vector>
#include <filesystem>

#include "../Service/Service.hpp"

class ServiceParser {
public:
    ServiceParser();
    ~ServiceParser();

    const std::vector<Service> ParseServices();

private:
    void OpenFile();
    void CloseFile();


private:
    std::fstream m_file;
    const std::filesystem::path m_path;
};

#endif // !SERVICE_PARSER_HPP