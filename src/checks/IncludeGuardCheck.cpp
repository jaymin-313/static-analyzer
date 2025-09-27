#include "checks/IncludeGuardCheck.h"
#include <sstream>
#include <string>

namespace checks
{

    void IncludeGuardCheck::run(const std::string &source,
                                const std::string &filename,
                                std::vector<analyzer::ReportItem> &report)
    {
        // Only check .h or .hpp files
        if (filename.size() < 2)
            return;
        if (!(filename.substr(filename.size() - 2) == ".h" ||
              (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".hpp")))
            return;

        std::istringstream iss(source);
        std::string line;
        bool hasPragmaOnce = false;
        bool hasIfndef = false;
        bool hasDefine = false;

        while (std::getline(iss, line))
        {
            std::string trimmed = line;
            trimmed.erase(trimmed.begin(), std::find_if(trimmed.begin(), trimmed.end(),
                                                        [](unsigned char c)
                                                        { return !std::isspace(c); }));
            if (trimmed.find("#pragma once") != std::string::npos)
                hasPragmaOnce = true;
            if (trimmed.find("#ifndef") != std::string::npos)
                hasIfndef = true;
            if (trimmed.find("#define") != std::string::npos)
                hasDefine = true;
        }

        if (!(hasPragmaOnce || (hasIfndef && hasDefine)))
        {
            report.push_back({filename, 1, "Header missing include guard (#pragma once or #ifndef/#define)"});
        }
    }

} // namespace checks
