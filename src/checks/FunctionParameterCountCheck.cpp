#include "checks/FunctionParameterCountCheck.h"
#include <sstream>
#include <string>
#include <algorithm>

namespace checks
{

    void FunctionParameterCountCheck::run(const std::string &source,
                                          const std::string &filename,
                                          std::vector<analyzer::ReportItem> &report)
    {
        std::istringstream iss(source);
        std::string line;
        int lineno = 0;

        while (std::getline(iss, line))
        {
            ++lineno;
            std::string trimmed = line;
            trimmed.erase(trimmed.begin(), std::find_if(trimmed.begin(), trimmed.end(),
                                                        [](unsigned char c)
                                                        { return !std::isspace(c); }));

            if (trimmed.find('(') != std::string::npos &&
                trimmed.find(';') == std::string::npos &&
                trimmed.find("class") == std::string::npos &&
                trimmed.find("struct") == std::string::npos)
            {

                size_t start = trimmed.find('(');
                size_t end = trimmed.find(')', start);
                if (end != std::string::npos)
                {
                    std::string params = trimmed.substr(start + 1, end - start - 1);
                    int count = 0;
                    bool nonEmpty = false;
                    for (char c : params)
                    {
                        if (c == ',')
                        {
                            count++;
                            nonEmpty = true;
                        }
                        else if (!isspace(c))
                            nonEmpty = true;
                    }
                    if (nonEmpty)
                        count++; // count last param
                    if (count > maxParams_)
                    {
                        report.push_back({filename, lineno,
                                          "Function has too many parameters (" + std::to_string(count) + ")"});
                    }
                }
            }
        }
    }

} // namespace checks
