#include "checks/TodoCheck.h"
#include <sstream>

namespace checks
{

    void TodoCheck::run(const std::string &source,
                        const std::string &filename,
                        std::vector<analyzer::ReportItem> &report)
    {
        std::istringstream iss(source);
        std::string line;
        int lineno = 0;
        while (std::getline(iss, line))
        {
            ++lineno;
            auto pos = line.find("TODO");
            if (pos != std::string::npos)
            {
                analyzer::ReportItem item;
                item.file = filename;
                item.line = lineno;
                item.message = "Found TODO: " + line;
                report.push_back(std::move(item));
            }
        }
    }

} // namespace checks