#include "checks/MagicNumberCheck.h"
#include <sstream>
#include <cctype>

namespace checks
{

    void MagicNumberCheck::run(const std::string &source,
                               const std::string &filename,
                               std::vector<analyzer::ReportItem> &report)
    {
        std::istringstream iss(source);
        std::string line;
        int lineno = 0;

        while (std::getline(iss, line))
        {
            ++lineno;

            size_t i = 0;
            while (i < line.size())
            {
                // Skip non-digits
                if (!std::isdigit(line[i]))
                {
                    ++i;
                    continue;
                }

                // Parse the number
                size_t start = i;
                while (i < line.size() && std::isdigit(line[i]))
                    ++i;
                std::string number = line.substr(start, i - start);

                // Skip trivial numbers
                if (number != "0" && number != "1")
                {
                    report.push_back({filename, lineno, "Magic number detected: " + number});
                }
            }
        }
    }

} // namespace checks
