#include "checks/LongFunctionCheck.h"
#include <sstream>
#include <string>
#include <algorithm>

namespace checks
{

    void LongFunctionCheck::run(const std::string &source,
                                const std::string &filename,
                                std::vector<analyzer::ReportItem> &report)
    {
        std::istringstream iss(source);
        std::string line;
        int lineno = 0;

        bool inFunction = false;
        int funcStartLine = 0;
        int braceDepth = 0;
        int funcLineCount = 0;
        bool potentialFunction = false;

        while (std::getline(iss, line))
        {
            ++lineno;

            // Remove leading spaces
            std::string trimmed = line;
            trimmed.erase(trimmed.begin(), std::find_if(trimmed.begin(), trimmed.end(),
                                                        [](unsigned char c)
                                                        { return !std::isspace(c); }));

            // Skip empty lines or comments
            if (trimmed.empty() || trimmed.substr(0, 2) == "//")
                continue;

            // Detect potential function signature
            if (!inFunction && trimmed.find('(') != std::string::npos &&
                trimmed.find(';') == std::string::npos &&
                trimmed.find("class") == std::string::npos &&
                trimmed.find("struct") == std::string::npos)
            {
                potentialFunction = true;
                funcStartLine = lineno;
                funcLineCount = 1;
                braceDepth = std::count(line.begin(), line.end(), '{') - std::count(line.begin(), line.end(), '}');
                if (braceDepth > 0)
                {
                    inFunction = true; // opening brace on same line
                    potentialFunction = false;
                }
                continue;
            }

            // If potential function signature, check next line for opening brace
            if (potentialFunction)
            {
                braceDepth += std::count(line.begin(), line.end(), '{') - std::count(line.begin(), line.end(), '}');
                funcLineCount++;
                if (braceDepth > 0)
                {
                    inFunction = true;
                    potentialFunction = false;
                }
                continue;
            }

            // If inside function
            if (inFunction)
            {
                funcLineCount++;
                braceDepth += std::count(line.begin(), line.end(), '{');
                braceDepth -= std::count(line.begin(), line.end(), '}');

                if (braceDepth <= 0)
                {
                    if (funcLineCount > maxLines_)
                    {
                        report.push_back({filename, funcStartLine,
                                          "Function exceeds max lines (" + std::to_string(funcLineCount) + ")"});
                    }
                    inFunction = false;
                }
            }
        }
    }

} // namespace checks
