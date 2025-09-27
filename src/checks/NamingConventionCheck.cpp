#include "checks/NamingConventionCheck.h"
#include <sstream>
#include <cctype>

namespace checks
{

    bool NamingConventionCheck::isPascalCase(const std::string &name) const
    {
        if (name.empty())
            return false;
        if (!std::isupper(name[0]))
            return false;
        for (char c : name)
        {
            if (!std::isalnum(c))
                return false;
        }
        return true;
    }

    bool NamingConventionCheck::isCamelCase(const std::string &name) const
    {
        if (name.empty())
            return false;
        if (!std::islower(name[0]))
            return false;
        for (char c : name)
        {
            if (!std::isalnum(c))
                return false;
        }
        return true;
    }

    bool NamingConventionCheck::isSnakeCase(const std::string &name) const
    {
        if (name.empty())
            return false;
        if (!std::islower(name[0]))
            return false;
        for (char c : name)
        {
            if (!(std::islower(c) || std::isdigit(c) || c == '_'))
                return false;
        }
        return true;
    }

    void NamingConventionCheck::run(const std::string &source,
                                    const std::string &filename,
                                    std::vector<analyzer::ReportItem> &report)
    {
        std::istringstream iss(source);
        std::string line;
        int lineno = 0;

        while (std::getline(iss, line))
        {
            ++lineno;

            // Simple line scanning instead of regex

            // Class
            size_t classPos = line.find("class ");
            if (classPos != std::string::npos)
            {
                size_t nameStart = classPos + 6;
                while (nameStart < line.size() && std::isspace(line[nameStart]))
                    ++nameStart;
                size_t nameEnd = nameStart;
                while (nameEnd < line.size() && (std::isalnum(line[nameEnd]) || line[nameEnd] == '_'))
                    ++nameEnd;
                std::string className = line.substr(nameStart, nameEnd - nameStart);
                if (!isPascalCase(className))
                {
                    report.push_back({filename, lineno,
                                      "Class name '" + className + "' should be PascalCase"});
                }
            }

            // Function
            size_t parenPos = line.find('(');
            size_t spacePos = line.find(' ');
            if (parenPos != std::string::npos && spacePos != std::string::npos && spacePos < parenPos)
            {
                std::string funcName = line.substr(spacePos + 1, parenPos - spacePos - 1);
                if (!funcName.empty() && !isCamelCase(funcName))
                {
                    report.push_back({filename, lineno,
                                      "Function name '" + funcName + "' should be camelCase"});
                }
            }

            // Variable (very naive: int, double, float, auto, string)
            std::string types[] = {"int ", "double ", "float ", "auto ", "std::string "};
            for (auto &t : types)
            {
                size_t typePos = line.find(t);
                if (typePos != std::string::npos)
                {
                    size_t nameStart = typePos + t.size();
                    while (nameStart < line.size() && std::isspace(line[nameStart]))
                        ++nameStart;
                    size_t nameEnd = nameStart;
                    while (nameEnd < line.size() && (std::isalnum(line[nameEnd]) || line[nameEnd] == '_'))
                        ++nameEnd;
                    std::string varName = line.substr(nameStart, nameEnd - nameStart);
                    if (!varName.empty() && !isSnakeCase(varName))
                    {
                        report.push_back({filename, lineno,
                                          "Variable name '" + varName + "' should be snake_case"});
                    }
                }
            }
        }
    }

} // namespace checks
